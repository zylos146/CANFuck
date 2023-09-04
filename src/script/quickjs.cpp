#include "task.hpp"
#include "script/quickjs.hpp"


static JSValue js_motor_onAtTarget(
  JSContext *ctx, JSValueConst this_val,
  int argc, JSValueConst *argv
) {

}

static JSValue js_motor_onMotionStopped(
  JSContext *ctx, JSValueConst this_val,
  int argc, JSValueConst *argv
) {

}

static JSValue js_motor_goTo(
  JSContext *ctx, JSValueConst this_val,
  int argc, JSValueConst *argv
) {
  JSValue position, velocity, acceleration;

  if (argc > 0) {
    position = argv[0];
  }

  if (argc > 1) {
    velocity = argv[1];
  }

  if (argc > 2) {
    acceleration = argv[2];
  }
}

void script_task_eventloop(void* param) {
  JSContext *ctx = reinterpret_cast<JSContext*>(param);
  int err;

  while(true) {
      err = JS_ExecutePendingJob(JS_GetRuntime(ctx), &ctx);
      if (err <= 0) {
          if (err < 0) {
              //js_std_dump_error(ctx);
          }
          break;
      }
  }
}

void script_start(JSContext *ctx) {
  // Define Motor global
  JSValue jsGlobal = JS_GetGlobalObject(ctx);
  JSValue jsMotor = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, jsMotor, "onAtTarget", JS_NewCFunction(ctx, js_motor_onAtTarget, "onAtTarget", 1));
  JS_SetPropertyStr(ctx, jsMotor, "goTo", JS_NewCFunction(ctx, js_motor_goTo, "goTo", 1));
  JS_SetPropertyStr(ctx, jsGlobal, "motor", jsMotor);

  xTaskCreatePinnedToCore(
    &script_task_eventloop, TASK_JS_EVENT_LOOP_NAME, 
    TASK_JS_EVENT_LOOP_STACK_SIZE, 
    static_cast<void*>(ctx), 
    TASK_JS_EVENT_LOOP_PRIORITY,
    NULL, 1
  );
}

/* TODO - Why is this erroring out?
static const JSCFunctionListEntry tjs_os_funcs[] = {
    JS_CONST(AF_INET),
    JS_CONST(AF_INET6),
    JS_CONST(AF_UNSPEC),
    JS_CONST(STDIN_FILENO),
    JS_CONST(STDOUT_FILENO),
    JS_CONST(STDERR_FILENO),
    JS_CFUNC_DEF("goTo", 1, js_motor_go_to),

    JS_CFUNC_DEF("hrtimeMs", 0, tjs_hrtime_ms),
    JS_CFUNC_DEF("uname", 0, tjs_uname),
    JS_CFUNC_DEF("uptime", 0, tjs_uptime),
    JS_CFUNC_DEF("guessHandle", 1, tjs_guess_handle),
    JS_CFUNC_DEF("getenv", 0, tjs_getenv),
};
*/
void script_init() {
  // QuickJS is not multi-thread safe, thus we pin to Core 1 for non-critical execution
  //xTaskCreatePinnedToCore(&script_start, "script_start", 4096 * 4, NULL, 2, NULL, 1);
}
