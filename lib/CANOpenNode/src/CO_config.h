#pragma once

#define CAN_TX_IO (34)  /** CAN TX pin */
#define CAN_RX_IO (35) /** CAN RX pin */
#define CAN_TX_QUEUE_LENGTH (5) /** CAN TX buffer size */
#define CAN_RX_QUEUE_LENGTH (5) /** CAN RX buffer size */

#define CO_CAN_PSEUDO_INTERRUPT_INTERVAL (1000) /** CAN "Interrupt" Interval in μs*/
#define CO_MAIN_TASK_INTERVAL (1000)   /* Interval of tmrTask thread in microseconds */

#define CAN_MS_TO_WAIT (50000) /*CAN TX/RX Timeout value*/
