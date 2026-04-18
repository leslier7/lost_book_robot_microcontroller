// Robbie Leslie
// April 2026

#if defined(ROS) || defined(ROS_DEBUG)
#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <lost_book_msgs/msg/tof.h>
#include <lost_book_msgs/msg/light.h>

#include "DistanceFunctions.h"
#include "LightFunctions.h"

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

#define EXECUTE_EVERY_N_MS(MS, X)  do { \
  static volatile int64_t init = -1; \
  if (init == -1) { init = uxr_millis();} \
  if (uxr_millis() - init > MS) { X; init = uxr_millis();} \
} while (0)

static rcl_publisher_t tofPublisher;
static rcl_subscription_t lightSubscriber;
static lost_book_msgs__msg__Tof tofMsg;
static lost_book_msgs__msg__Light lightMsg;
static rclc_executor_t executor;
static rclc_support_t support;
static rcl_allocator_t allocator;
static rcl_node_t node;
static rcl_timer_t timer;

enum states {
    WAITING_AGENT,
    AGENT_AVAILABLE,
    AGENT_CONNECTED,
    AGENT_DISCONNECTED
} state;

static void error_loop() {
    while (1) {
        delay(100);
    }
}

static void timer_callback(rcl_timer_t *inputTimer, int64_t last_call_time) {
    RCLC_UNUSED(last_call_time);
    if (inputTimer != NULL) {
        tofMsg.tof_front = getDistance(FRONT);
        tofMsg.tof_left  = getDistance(LEFT);
        tofMsg.tof_right = getDistance(RIGHT);
        tofMsg.tof_back  = getDistance(BACK);
        tofMsg.tof_extra = getDistance(EXTRA);
        RCSOFTCHECK(rcl_publish(&tofPublisher, &tofMsg, NULL));
    }
}

static void light_subscription_callback(const void *msgin) {
    const auto *msg = (const lost_book_msgs__msg__Light *) msgin;
    setLight(msg->state == lost_book_msgs__msg__Light__ON);
}

bool create_entities() {
    allocator = rcl_get_default_allocator();

    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
    RCCHECK(rclc_node_init_default(&node, "lost_book_robot_node", "", &support));

    RCCHECK(rclc_publisher_init_best_effort(
        &tofPublisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(lost_book_msgs, msg, Tof),
        "tof"));

    RCCHECK(rclc_timer_init_default(
        &timer,
        &support,
        RCL_MS_TO_NS(100),
        timer_callback));

    RCCHECK(rclc_subscription_init_default(
        &lightSubscriber,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(lost_book_msgs, msg, Light),
        "light"));

    executor = rclc_executor_get_zero_initialized_executor();
    RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
    RCCHECK(rclc_executor_add_timer(&executor, &timer));
    RCCHECK(rclc_executor_add_subscription(&executor, &lightSubscriber, &lightMsg,
        &light_subscription_callback, ON_NEW_DATA));

    return true;
}

void destroy_entities() {
    rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
    (void) rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

    RCSOFTCHECK(rcl_publisher_fini(&tofPublisher, &node));
    RCSOFTCHECK(rcl_subscription_fini(&lightSubscriber, &node));
    RCSOFTCHECK(rcl_timer_fini(&timer));
    RCSOFTCHECK(rclc_executor_fini(&executor));
    RCSOFTCHECK(rcl_node_fini(&node));
    RCSOFTCHECK(rclc_support_fini(&support));
}

void microRosTick() {
    switch (state) {
        case WAITING_AGENT:
            EXECUTE_EVERY_N_MS(500, state = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_AVAILABLE : WAITING_AGENT;);
            break;
        case AGENT_AVAILABLE:
            state = (true == create_entities()) ? AGENT_CONNECTED : WAITING_AGENT;
            if (state == WAITING_AGENT) {
                destroy_entities();
            }
            break;
        case AGENT_CONNECTED:
            EXECUTE_EVERY_N_MS(200, state = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_CONNECTED : AGENT_DISCONNECTED;);
            if (state == AGENT_CONNECTED) {
                rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
            }
            break;
        case AGENT_DISCONNECTED:
            destroy_entities();
            state = WAITING_AGENT;
            break;
        default:
            break;
    }
}

bool microRosSetup() {
    set_microros_serial_transports(Serial);
    delay(2000);
    state = WAITING_AGENT;
    return true;
}

#endif
