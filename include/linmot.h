#ifndef LINMOT_H
#define LINMOT_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// TODO - Somehow integrate into main app task? What happens on app/comm reset?
// TODO - Ensure 16bit mapping is correct in Linmot
#define LINMOT_CONTROL_SWITCH_ON ((uint16_t)(1 << 0))
#define LINMOT_CONTROL_ERROR_ACKNOWLEDGE ((uint16_t)(1 << 7))
#define LINMOT_CONTROL_HOME ((uint16_t)(1 << 11))

#define LINMOT_STATUS_OPERATION_ENABLED (1 << 0)
#define LINMOT_STATUS_SWITCH_ON (1 << 1)
#define LINMOT_STATUS_ERROR (1 << 3)
#define LINMOT_STATUS_VOLTAGE_ENABLE (1 << 4)
#define LINMOT_STATUS_WARNING (1 << 7)
#define LINMOT_STATUS_AT_TARGET (1 << 10)
#define LINMOT_STATUS_HOMED (1 << 11)
#define LINMOT_STATUS_FATAL_ERROR (1 << 12)
#define LINMOT_STATUS_MOTION_ACTIVE (1 << 13)

#define LINMOT_WARN_MOTOR_HOT_SENSOR (1 << 0)
#define LINMOT_WARN_MOTOR_SHORT_TIME_OVERLOAD (1 << 1)
#define LINMOT_WARN_MOTOR_SUPPLY_VOLTAGE_LOW (1 << 2)
#define LINMOT_WARN_MOTOR_SUPPLY_VOLTAGE_HIGH (1 << 3)
#define LINMOT_WARN_MOTOR_DRIVE_HOT (1 << 6)
#define LINMOT_WARN_MOTOR_NOT_HOMED (1 << 7)

#define LINMOT_ERROR_CANBUS_GUARD_TIMEOUT (0xCD)

// Accepts four 16bit parameters 
// TODO - Define parameters better
#define LINMOT_CMD_VAI_16BIT_GO_TO_POS (0x0900)

#define MOTION_STATE_INITIALIZING 0x00
#define MOTION_STATE_
#define MOTION_STATE_HOMING

void linmot_register_tasks();
void linmot_control_add_flag(uint16_t flag);
void linmot_control_remove_flag(uint16_t flag);
void linmot_send_cmd(uint16_t cmd, uint16_t parameter_a, uint16_t parameter_b, uint16_t parameter_c, uint16_t parameter_d);

#endif