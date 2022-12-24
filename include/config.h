#ifndef CONFIG_H
#define CONFIG_H

// #### Stroke Engine #### //
#define CONTROLLER_USED false

#define WIFI_SSID ""
#define WIFI_PASS ""

#define MOTOR_LINMOT 1
#define MOTOR_STEPPER 2
#define MOTOR_MOCK 3
#define MOTOR_USED 3

// #### CANOpen #### //
#define CAN_TX_IO (35)  /** CAN TX pin */
#define CAN_RX_IO (36) /** CAN RX pin */

#define CAN_BITRATE 500    /* kbit */
#define NODE_ID_SELF 0x01
#define NODE_ID_LINMOT 0x3F

#define CAN_B1100_OD 1
#define CAN_E1100_OD 1
#define CAN_A1100_OD 2
#define CAN_OD_USED CAN_A1100_OD

// #### Logging #### //
// TODO - Doesn't do anything yet
#define FRAMEWORK_ARDUINO 1
#define FRAMEWORK_ESP_IDF 2
#define FRAMEWORK_USED FRAMEWORK_ESP_IDF

// #### FreeRTOS #### //
#define BOOT_WAIT 2000
#define MAIN_WAIT 100 /** Time in ms between every main loop cycle */

// #### Communication #### //
//#define UART_PORT UART_NUM_0
//#define UART_TX_PIN GPIO_NUM_33 /** Serial TX Pinnummber */
//#define UART_RX_PIN GPIO_NUM_32 /** Serial RX Pinnummber */
#define UART_SPEED 115200

#define I2C_SDA_PIN 36
#define I2C_SCL_PIN 37

#endif