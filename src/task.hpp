#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// TODO - Convert all tasks in system to this style of defines
// Should help with keeping task priority straight
#define TASK_JS_EVENT_LOOP_NAME "js_event_loop"
#define TASK_JS_EVENT_LOOP_STACK_SIZE 4096*4
#define TASK_JS_EVENT_LOOP_PRIORITY 16