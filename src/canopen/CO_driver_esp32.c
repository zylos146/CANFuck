/*
 * CAN module object for generic microcontroller.
 *
 * This file is a template for other microcontrollers.
 *
 * @file        CO_driver.c
 * @ingroup     CO_driver
 * @author      Janez Paternoster
 * @author      Alexander Miller
 * @author      Mathias Parys
 * @copyright   2004 - 2020 Janez Paternoster
 *
 * This file is part of CANopenNode, an opensource CANopen Stack.
 * Project home page is <https://github.com/CANopenNode/CANopenNode>.
 * For more information on CANopen see <http://www.can-cia.org/>.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "soc/soc.h"

#include "CANopen.h"
#include "CO_config_target.h"
#include "301/CO_driver.h"
#include "301/CO_Emergency.h"

#include "driver/twai.h"
#include "driver/gpio.h"

#include "soc/gpio_periph.h"
#include "hal/gpio_hal.h"

CO_CANmodule_t *CANmodulePointer = NULL;

// CAN Timing configuration
static twai_timing_config_t timingConfig = TWAI_TIMING_CONFIG_500KBITS();   // Set Baudrate to 1Mbit
                                                                            // CAN Filter configuration
static twai_filter_config_t filterConfig = TWAI_FILTER_CONFIG_ACCEPT_ALL(); // Disable Message Filter
                                                                            // CAN General configuration

static twai_general_config_t generalConfig = {
    .mode = TWAI_MODE_NORMAL,
    .tx_io = CAN_TX_IO,                                    /*TX IO Pin (CO_config.h)*/
    .rx_io = CAN_RX_IO,                                    /*RX IO Pin (CO_config.h)*/
    .clkout_io = TWAI_IO_UNUSED,                           /*No clockout pin*/
    .bus_off_io = TWAI_IO_UNUSED,                          /*No busoff pin*/
    .tx_queue_len = CAN_TX_QUEUE_LENGTH,                   /*ESP TX Buffer Size (CO_config.h)*/
    .rx_queue_len = CAN_RX_QUEUE_LENGTH,                   /*ESP RX Buffer Size (CO_config.h)*/
    .alerts_enabled = TWAI_ALERT_ALL | TWAI_ALERT_AND_LOG, /*Disable CAN Alarms TODO: Enable for CO_CANverifyErrors*/
    .clkout_divider = 0,
    .intr_flags = ESP_INTR_FLAG_LEVEL1, /*CAN Interrupt Priority*/
};                                      /*No Clockout*/

// Timer Interrupt Configuration
const esp_timer_create_args_t CO_CANinterruptArgs = {
    .callback = &CO_CANinterrupt, /*Timer Interrupt Callback */
    .name = "CO_CANinterrupt"};   /* Optional Task Name for debugging */

// Timer Handle
esp_timer_handle_t CO_CANinterruptPeriodicTimer;
uint32_t CO_txTraceId = 0;

/******************************************************************************/
void CO_CANsetConfigurationMode(void *CANdriverState)
{
  /* Put CAN module in configuration mode */
  (void)CANdriverState;
}

/******************************************************************************/
void CO_CANsetNormalMode(CO_CANmodule_t *CANmodule)
{

  // gpio_hal_iomux_func_sel(GPIO_PIN_MUX_REG[CAN_TX_IO], PIN_FUNC_GPIO);
  // gpio_hal_iomux_func_sel(GPIO_PIN_MUX_REG[CAN_RX_IO], PIN_FUNC_GPIO);

  ESP_LOGE("CANOpen.init", "install");
  /*Install CAN driver*/
  ESP_ERROR_CHECK(twai_driver_install(&generalConfig, &timingConfig, &filterConfig));
  /*Start CAN Controller*/
  ESP_LOGE("CANOpen.init", "start");
  ESP_ERROR_CHECK(twai_start());

  ESP_LOGE("CANOpen.init", "add handler");

  /*WORKAROUND: INTERRUPT ALLOCATION FÜR CAN NICHT MÖGLICH DA BEREITS IM IDF TREIBER VERWENDET*/
  /* Configure Timer interrupt function for execution every CO_CAN_PSEUDO_INTERRUPT_INTERVAL */
  ESP_ERROR_CHECK(esp_timer_create(&CO_CANinterruptArgs, &CO_CANinterruptPeriodicTimer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(CO_CANinterruptPeriodicTimer, CO_CAN_PSEUDO_INTERRUPT_INTERVAL));
  /*Set Canmodule to normal mode*/
  CANmodule->CANnormal = true;
}

/******************************************************************************/
CO_ReturnError_t CO_CANmodule_init(
    CO_CANmodule_t *CANmodule,
    void *CANptr,
    CO_CANrx_t rxArray[],
    uint16_t rxSize,
    CO_CANtx_t txArray[],
    uint16_t txSize,
    uint16_t CANbitRate)
{
  CANmodulePointer = CANmodule;

  /* verify arguments */
  if (CANmodule == NULL || rxArray == NULL || txArray == NULL)
  {
    ESP_LOGE("CO_CANmodule_init", "Verify arguments! (Nullpointer in CANmodule|rxArray|txArray)");
    return CO_ERROR_ILLEGAL_ARGUMENT;
  }

  /* Configure object variables */
  CANmodule->CANptr = CANptr;
  CANmodule->rxSize = rxSize;
  CANmodule->rxArray = rxArray;
  CANmodule->txSize = txSize;
  CANmodule->txArray = txArray;
  CANmodule->useCANrxFilters = false;
  CANmodule->firstCANtxMessage = true;
  CANmodule->CANnormal = false;
  CANmodule->bufferInhibitFlag = false;
  CANmodule->CANtxCount = 0U;
  CANmodule->errOld = 0U;

  /*Init RX-Array*/
  for (uint16_t i = 0U; i < rxSize; i++)
  {
    CANmodule->rxArray[i].ident = 0U;
    CANmodule->rxArray[i].mask = (uint16_t)0xFFFFFFFFU;
    CANmodule->rxArray[i].object = NULL;
    CANmodule->rxArray[i].CANrx_callback = NULL;
  }
  /*Init TX-Array*/
  for (uint16_t i = 0U; i < txSize; i++)
  {
    CANmodule->txArray[i].bufferFull = false;
  }

  /* Configure CAN module registers */
  generalConfig.mode = TWAI_MODE_NORMAL;
  generalConfig.tx_io = CAN_TX_IO;
  generalConfig.rx_io = CAN_RX_IO;
  generalConfig.clkout_io = TWAI_IO_UNUSED;
  generalConfig.bus_off_io = TWAI_IO_UNUSED;
  generalConfig.tx_queue_len = CAN_TX_QUEUE_LENGTH;
  generalConfig.rx_queue_len = CAN_RX_QUEUE_LENGTH;
  generalConfig.alerts_enabled = TWAI_ALERT_ALL;
  generalConfig.clkout_divider = 0;

  /* Configure CAN timing */
  switch (CANbitRate)
  {
  case 25:
    // Set Baudrate to 25kbit;
    timingConfig.brp = 128;
    timingConfig.tseg_1 = 16;
    timingConfig.tseg_2 = 8;
    timingConfig.sjw = 3;
    timingConfig.triple_sampling = false;
    break;
  case 50:
    // Set Baudrate to 50kbit;
    timingConfig.brp = 80;
    timingConfig.tseg_1 = 15;
    timingConfig.tseg_2 = 4;
    timingConfig.sjw = 3;
    timingConfig.triple_sampling = false;
    break;
  case 100:
    // Set Baudrate to 100kbit;
    timingConfig.brp = 40;
    timingConfig.tseg_1 = 15;
    timingConfig.tseg_2 = 4;
    timingConfig.sjw = 3;
    timingConfig.triple_sampling = false;
    break;
  case 125:
    // Set Baudrate to 125kbit;
    timingConfig.brp = 32;
    timingConfig.tseg_1 = 15;
    timingConfig.tseg_2 = 4;
    timingConfig.sjw = 3;
    timingConfig.triple_sampling = false;
    break;
  case 250:
    // Set Baudrate to 250kbit;
    timingConfig.brp = 16;
    timingConfig.tseg_1 = 15;
    timingConfig.tseg_2 = 4;
    timingConfig.sjw = 3;
    timingConfig.triple_sampling = false;
    break;
  case 500:
    // Set Baudrate to 500kbit;
    timingConfig.brp = 8;
    timingConfig.tseg_1 = 15;
    timingConfig.tseg_2 = 4;
    timingConfig.sjw = 3;
    timingConfig.triple_sampling = false;
    break;
  case 800:
    // Set Baudrate to 800kbit;
    timingConfig.brp = 4;
    timingConfig.tseg_1 = 16;
    timingConfig.tseg_2 = 8;
    timingConfig.sjw = 3;
    timingConfig.triple_sampling = false;
    break;
  case 1000:
    // Set Baudrate to 1Mbit;
    timingConfig.brp = 4;
    timingConfig.tseg_1 = 15;
    timingConfig.tseg_2 = 4;
    timingConfig.sjw = 3;
    timingConfig.triple_sampling = false;
    break;
  default:
    ESP_LOGE("CO_driver", "%d => Invalid Baudrate! Using 1Mbit as default!", CANbitRate);
    // Set Baudrate to 1Mbit;
    timingConfig.brp = 4;
    timingConfig.tseg_1 = 15;
    timingConfig.tseg_2 = 4;
    timingConfig.sjw = 3;
    timingConfig.triple_sampling = false;
  }

  /* Configure CAN module hardware filters */
  if (CANmodule->useCANrxFilters)
  {
    /* CAN module filters are used, they will be configured with */
    /* CO_CANrxBufferInit() functions, called by separate CANopen */
    /* init functions. */
    /* Configure all masks so, that received message must match filter */

    // Don't filter messages
    ESP_LOGE("CO_driver", "RxFilters are active, but no filter configured.");
    filterConfig.acceptance_code = 0;
    filterConfig.acceptance_mask = 0xFFFFFFFF;
    filterConfig.single_filter = true;
  }
  else
  {
    /* CAN module filters are not used, all messages with standard 11-bit */
    /* identifier will be received */
    /* Configure mask 0 so, that all messages with standard identifier are accepted */
    // Don't filter messages
    filterConfig.acceptance_code = 0;
    filterConfig.acceptance_mask = 0xFFFFFFFF;
    filterConfig.single_filter = true;
  }
  return CO_ERROR_NO;
}

/******************************************************************************/
void CO_CANmodule_disable(CO_CANmodule_t *CANmodule)
{
  /* turn off the module */
  ESP_ERROR_CHECK(twai_stop());
}

/******************************************************************************/
CO_ReturnError_t CO_CANrxBufferInit(
    CO_CANmodule_t *CANmodule,
    uint16_t index,
    uint16_t ident,
    uint16_t mask,
    bool_t rtr,
    void *object,
    void (*CANrx_callback)(void *object, void *message))
{
  CO_ReturnError_t ret = CO_ERROR_NO;

  if ((CANmodule != NULL) && (object != NULL) && (CANrx_callback != NULL) && (index < CANmodule->rxSize))
  {
    /* buffer, which will be configured */
    CO_CANrx_t *buffer = &CANmodule->rxArray[index];

    /* Configure object variables */
    buffer->object = object;
    buffer->CANrx_callback = CANrx_callback;

    /* CAN identifier and CAN mask, bit aligned with CAN module. Different on different microcontrollers. */
    buffer->ident = ident & 0x07FFU;
    if (rtr)
    {
      buffer->ident |= 0x0800U;
    }
    buffer->mask = (mask & 0x07FFU) | 0x0800U;

    /* Set CAN hardware module filter and mask. */
    if (CANmodule->useCANrxFilters)
    {
      ESP_LOGE("CO_CANrxBufferInit", "No driver implementation for Filters");
    }
  }
  else
  {
    ESP_LOGE("CO_CANrxBufferInit", "((CANmodule!=NULL) && (object!=NULL) && (pFunct!=NULL) && (index < CANmodule->rxSize))==FALSE");
    ret = CO_ERROR_ILLEGAL_ARGUMENT;
  }

  return ret;
}

/******************************************************************************/
CO_CANtx_t *CO_CANtxBufferInit(
    CO_CANmodule_t *CANmodule,
    uint16_t index,
    uint16_t ident,
    bool_t rtr,
    uint8_t noOfBytes,
    bool_t syncFlag)
{
  CO_CANtx_t *buffer = NULL;

  if ((CANmodule != NULL) && (index < CANmodule->txSize))
  {
    /* get specific buffer */
    buffer = &CANmodule->txArray[index];

    /* CAN identifier, DLC and rtr, bit aligned with CAN module transmit buffer. */
    /* Convert data from library message into esp message values */
    buffer->ident = ((uint32_t)ident & 0x07FFU); /* Set Message ID (Standard frame), delete other informations */
    buffer->rtr = rtr;                           /* Set RTR Flag */
    buffer->DLC = noOfBytes;                     /* Set number of bytes */
    buffer->bufferFull = false;                  /* Set buffer full flag */
    buffer->syncFlag = syncFlag;                 /* Set sync flag */
  }
  else
  {
    ESP_LOGE("CO_CANtxBufferInit", "CANmodule not initialized or index out of bound of txSize");
  }
  return buffer;
}

/******************************************************************************/
CO_ReturnError_t CO_CANsend(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer)
{
  CO_ReturnError_t err = CO_ERROR_NO;
  twai_status_info_t esp_can_hw_status;                      /* Define variable for hardware status of esp can interface */
  ESP_ERROR_CHECK(twai_get_status_info(&esp_can_hw_status)); /* Get hardware status of esp can interface */

  /* Verify overflow */
  if (buffer->bufferFull)
  {
    err = CO_ERROR_TX_OVERFLOW;
    ESP_LOGE("CO_CANsend", "CAN-tx-buffer overflow 1");
    if (!CANmodule->firstCANtxMessage)
    {
      /* don't set error, if bootup message is still on buffers */
      // TODO - CO_errorReport((CO_EM_t *)CANmodule->em, CO_EM_CAN_TX_OVERFLOW, CO_EMC_CAN_OVERRUN, buffer->ident);
      // TODO - Temporary fix for buffer overflow if LinMot loses power
      esp_restart();
    }
  }

  CO_LOCK_CAN_SEND();
  /* if CAN TX buffer is free, copy message to it */
  if ((esp_can_hw_status.msgs_to_tx < CAN_TX_QUEUE_LENGTH) && (CANmodule->CANtxCount == 0))
  {
    uint32_t traceId = CO_txTraceId++;

    CANmodule->bufferInhibitFlag = buffer->syncFlag;
    twai_message_t temp_can_message; /* generate esp can message for transmission */
    /*MESSAGE MIT DATEN FÜLLEN*/
    temp_can_message.identifier = buffer->ident;     /* Set message-id in esp can message */
    temp_can_message.data_length_code = buffer->DLC; /* Set data length in esp can message */
    temp_can_message.flags = TWAI_MSG_FLAG_NONE;     /* reset all flags in esp can message */
    // temp_can_message.flags = TWAI_MSG_FLAG_SELF;
    if (buffer->rtr)
    {
      temp_can_message.flags |= TWAI_MSG_FLAG_RTR; /* set rtr-flag if needed */
    }
    for (uint8_t i = 0; i < buffer->DLC; i++)
    {
      temp_can_message.data[i] = buffer->data[i]; /* copy data from buffer in esp can message */
    }

    // ESP_LOGE("CANsend", "Trace: %u, ID: %d, Data %d,%d,%d,%d,%d,%d", traceId, temp_can_message.identifier, temp_can_message.data[0], temp_can_message.data[1], temp_can_message.data[2], temp_can_message.data[3], temp_can_message.data[4], temp_can_message.data[5]);

    /* Transmit esp can message.  */
    esp_err_t send_err = twai_transmit(&temp_can_message, pdMS_TO_TICKS(CAN_MS_TO_WAIT));
    if (send_err == ESP_OK)
    {
      // ESP_LOGE("CO_CANsend", "Trace: %u, Success", traceId);
      //  TODO - Filter log system
    }
    else
    {

      // Translate send_err into a human-readable string based on the following error codes:

      // #define ESP_OK          0       /*!< esp_err_t value indicating success (no error) */
      // #define ESP_FAIL        -1      /*!< Generic esp_err_t code indicating failure */

      // #define ESP_ERR_NO_MEM              0x101   /*!< Out of memory */
      // #define ESP_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
      // #define ESP_ERR_INVALID_STATE       0x103   /*!< Invalid state */
      // #define ESP_ERR_INVALID_SIZE        0x104   /*!< Invalid size */
      // #define ESP_ERR_NOT_FOUND           0x105   /*!< Requested resource not found */
      // #define ESP_ERR_NOT_SUPPORTED       0x106   /*!< Operation or feature not supported */
      // #define ESP_ERR_TIMEOUT             0x107   /*!< Operation timed out */
      // #define ESP_ERR_INVALID_RESPONSE    0x108   /*!< Received response was invalid */
      // #define ESP_ERR_INVALID_CRC         0x109   /*!< CRC or checksum was invalid *//
      // #define ESP_ERR_INVALID_VERSION     0x10A   /*!< Version was invalid */
      // #define ESP_ERR_INVALID_MAC         0x10B   /*!< MAC address was invalid */
      // #define ESP_ERR_NOT_FINISHED        0x10C   /*!< There are items remained to retrieve */

      // #define TWAI_ALERT_TX_IDLE                  0x00000001  /**< Alert(1): No more messages to transmit */
      // #define TWAI_ALERT_TX_SUCCESS               0x00000002  /**< Alert(2): The previous transmission was successful */
      // #define TWAI_ALERT_RX_DATA                  0x00000004  /**< Alert(4): A frame has been received and added to the RX queue */
      // #define TWAI_ALERT_BELOW_ERR_WARN           0x00000008  /**< Alert(8): Both error counters have dropped below error warning limit */
      // #define TWAI_ALERT_ERR_ACTIVE               0x00000010  /**< Alert(16): TWAI controller has become error active */
      // #define TWAI_ALERT_RECOVERY_IN_PROGRESS     0x00000020  /**< Alert(32): TWAI controller is undergoing bus recovery */
      // #define TWAI_ALERT_BUS_RECOVERED            0x00000040  /**< Alert(64): TWAI controller has successfully completed bus recovery */
      // #define TWAI_ALERT_ARB_LOST                 0x00000080  /**< Alert(128): The previous transmission lost arbitration */
      // #define TWAI_ALERT_ABOVE_ERR_WARN           0x00000100  /**< Alert(256): One of the error counters have exceeded the error warning limit */
      // #define TWAI_ALERT_BUS_ERROR                0x00000200  /**< Alert(512): A (Bit, Stuff, CRC, Form, ACK) error has occurred on the bus */
      // #define TWAI_ALERT_TX_FAILED                0x00000400  /**< Alert(1024): The previous transmission has failed (for single shot transmission) */
      // #define TWAI_ALERT_RX_QUEUE_FULL            0x00000800  /**< Alert(2048): The RX queue is full causing a frame to be lost */
      // #define TWAI_ALERT_ERR_PASS                 0x00001000  /**< Alert(4096): TWAI controller has become error passive */
      // #define TWAI_ALERT_BUS_OFF                  0x00002000  /**< Alert(8192): Bus-off condition occurred. TWAI controller can no longer influence bus */
      // #define TWAI_ALERT_RX_FIFO_OVERRUN          0x00004000  /**< Alert(16384): An RX FIFO overrun has occurred */
      // #define TWAI_ALERT_TX_RETRIED               0x00008000  /**< Alert(32768): An message transmission was cancelled and retried due to an errata workaround */
      // #define TWAI_ALERT_PERIPH_RESET             0x00010000  /**< Alert(65536): The TWAI controller was reset */
      // #define TWAI_ALERT_ALL                      0x0001FFFF  /**< Bit mask to enable all alerts during configuration */

      // TWAI_ALERT_ABOVE_ERR_WARN || TWAI_ALERT_BUS_ERROR || TWAI_ALERT_ERR_PASS || TWAI_ALERT_BUS_OFF
      err = CO_ERROR_TIMEOUT;
      uint32_t alerts = 0;
      esp_err_t alert_err = twai_read_alerts(&alerts, pdMS_TO_TICKS(CAN_MS_TO_WAIT));
      ESP_LOGE("CO_CANsend", "Trace: %u, Failed to queue message for transmission - send_err:%u alert:%u alert_err:%u", traceId, send_err, alerts, alert_err);
    }
  }
  /* if no buffer is free, message will be sent by interrupt */
  else
  {
    ESP_LOGE("CO_CANsend", "CAN-tx-buffer overflow 2 - %u - %u", esp_can_hw_status.msgs_to_tx, CANmodule->CANtxCount);
    // TODO - Fix this and have logic for bufferFull to be set to false
    // buffer->bufferFull = true;
    // CANmodule->CANtxCount++;
  }
  CO_UNLOCK_CAN_SEND();

  return err;
}

/******************************************************************************/
void CO_CANclearPendingSyncPDOs(CO_CANmodule_t *CANmodule)
{
  // TODO: Implement this function
  ESP_LOGE("CO_CANclearPendingSyncPDOs", "This function is NOT implemented!");
  uint32_t tpdoDeleted = 0U;

  // CO_LOCK_CAN_SEND();
  // /* Abort message from CAN module, if there is synchronous TPDO.
  //  * Take special care with this functionality. */
  // if(/*messageIsOnCanBuffer && */CANmodule->bufferInhibitFlag){
  //     /* clear TXREQ */
  //     CANmodule->bufferInhibitFlag = false;
  //     tpdoDeleted = 1U;
  // }
  // /* delete also pending synchronous TPDOs in TX buffers */
  // if(CANmodule->CANtxCount != 0U){
  //     uint16_t i;
  //     CO_CANtx_t *buffer = &CANmodule->txArray[0];
  //     for(i = CANmodule->txSize; i > 0U; i--){
  //         if(buffer->bufferFull){
  //             if(buffer->syncFlag){
  //                 buffer->bufferFull = false;
  //                 CANmodule->CANtxCount--;
  //                 tpdoDeleted = 2U;
  //             }
  //         }
  //         buffer++;
  //     }
  // }
  // CO_UNLOCK_CAN_SEND();

  if (tpdoDeleted != 0U)
  {
    // TODO - CO_errorReport((CO_EM_t *)CANmodule->em, CO_EM_TPDO_OUTSIDE_WINDOW, CO_EMC_COMMUNICATION, tpdoDeleted);
  }
}
// TODO: usefull error handling
/******************************************************************************/
void CO_CANverifyErrors(CO_CANmodule_t *CANmodule)
{

  uint16_t rxErrors, txErrors, overflow;
  // TODO - CO_EM_t *em = (CO_EM_t *)CANmodule->em;
  uint32_t err;

  /* get error counters from module. Id possible, function may use different way to
   * determine errors. */
  rxErrors = 0;
  txErrors = 0;
  overflow = 0;
  // rxErrors = CANmodule->txSize;
  // txErrors = CANmodule->txSize;
  // overflow = CANmodule->txSize;

  err = ((uint32_t)txErrors << 16) | ((uint32_t)rxErrors << 8) | overflow;

  if (CANmodule->errOld != err)
  {
    CANmodule->errOld = err;

    if (txErrors >= 256U)
    { /* bus off */
      ESP_LOGE("CO_CANverifyErrors", "can bus off");
      // TODO - CO_errorReport(em, CO_EM_CAN_TX_BUS_OFF, CO_EMC_BUS_OFF_RECOVERED, err);
    }
    else
    { /* not bus off */
      ESP_LOGE("CO_CANverifyErrors", "reset bus off");
      // TODO - CO_errorReset(em, CO_EM_CAN_TX_BUS_OFF, err);

      if ((rxErrors >= 96U) || (txErrors >= 96U))
      { /* bus warning */
        ESP_LOGE("CO_CANverifyErrors", "bus warning");
        // TODO - CO_errorReport(em, CO_EM_CAN_BUS_WARNING, CO_EMC_NO_ERROR, err);
      }

      if (rxErrors >= 128U)
      { /* RX bus passive */
        ESP_LOGE("CO_CANverifyErrors", "RX bus passive");
        // TODO - CO_errorReport(em, CO_EM_CAN_RX_BUS_PASSIVE, CO_EMC_CAN_PASSIVE, err);
      }
      else
      {
        ESP_LOGE("CO_CANverifyErrors", "reset RX bus passive");
        // TODO - CO_errorReset(em, CO_EM_CAN_RX_BUS_PASSIVE, err);
      }

      if (txErrors >= 128U)
      { /* TX bus passive */
        if (!CANmodule->firstCANtxMessage)
        {
          ESP_LOGE("CO_CANverifyErrors", "TX bus passive");
          // TODO - CO_errorReport(em, CO_EM_CAN_TX_BUS_PASSIVE, CO_EMC_CAN_PASSIVE, err);
        }
      }
      else
      {
        // TODO - bool_t isError = CO_isError(em, CO_EM_CAN_TX_BUS_PASSIVE);
        // TODO - if (isError)
        // TODO - {
        ESP_LOGE("CO_CANverifyErrors", "reset TX bus passive");
        // TODO - CO_errorReset(em, CO_EM_CAN_TX_BUS_PASSIVE, err);
        // TODO - CO_errorReset(em, CO_EM_CAN_TX_OVERFLOW, err);
        // TODO - }
      }

      if ((rxErrors < 96U) && (txErrors < 96U))
      { /* no error */
        ESP_LOGE("CO_CANverifyErrors", "no error");
        // TODO - CO_errorReset(em, CO_EM_CAN_BUS_WARNING, err);
      }
    }

    if (overflow != 0U)
    { /* CAN RX bus overflow */
      ESP_LOGE("CO_CANverifyErrors", "CAN RX bus overflow oO");
      // TODO - CO_errorReport(em, CO_EM_CAN_RXB_OVERFLOW, CO_EMC_CAN_OVERRUN, err);
    }
  }
}

/******************************************************************************/
void CO_CANinterrupt(void *args)
{
  CO_CANmodule_t *CANmodule = CANmodulePointer;
  twai_status_info_t esp_can_hw_status;                      /* Define variable for hardware status of esp can interface */
  ESP_ERROR_CHECK(twai_get_status_info(&esp_can_hw_status)); /* Get hardware status of esp can interface */
  /* receive interrupt */
  if (esp_can_hw_status.msgs_to_rx != 0)
  {
    twai_message_t temp_can_message; // ESP data type can message
    ESP_ERROR_CHECK(twai_receive(&temp_can_message, pdMS_TO_TICKS(CAN_MS_TO_WAIT)));

    /*
    ESP_LOGE(
      "CANreceive",
      "ID: %d , Data %d,%d,%d,%d,%d,%d",
      temp_can_message.identifier,
      temp_can_message.data[0],
      temp_can_message.data[1],
      temp_can_message.data[2],
      temp_can_message.data[3],
      temp_can_message.data[4],
      temp_can_message.data[5]
    );
    */

    CO_CANrxMsg_t rcvMsg;      /* pointer to received message in CAN module */
    uint16_t index;            /* index of received message */
    uint32_t rcvMsgIdent;      /* identifier of the received message */
    CO_CANrx_t *buffer = NULL; /* receive message buffer from CO_CANmodule_t object. */
    bool_t msgMatched = false;

    rcvMsg.ident = temp_can_message.identifier;
    /* check if rtr flag is set in esp can message*/
    if (temp_can_message.flags & TWAI_MSG_FLAG_RTR)
    {
      rcvMsg.ident += (1 << 12); /* Set RTR flag in library message */
    }
    rcvMsg.dlc = temp_can_message.data_length_code; /* Set data length in library message */
    for (uint8_t i = 0; i < temp_can_message.data_length_code; i++)
    {
      rcvMsg.data[i] = temp_can_message.data[i]; /* copy data from esp can message to library message */
    }

    rcvMsgIdent = rcvMsg.ident;
    if (CANmodule->useCANrxFilters)
    {
      ESP_LOGE("CO_CANinterrupt", "Filter system is not implemented");
      /* CAN module filters are used. Message with known 11-bit identifier has */
      /* been received */
      index = 0; /* get index of the received message here. Or something similar */
      if (index < CANmodule->rxSize)
      {
        buffer = &CANmodule->rxArray[index];
        /* verify also RTR */
        if (((rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U)
        {
          msgMatched = true;
        }
      }
    }
    else
    {
      /* CAN module filters are not used, message with any standard 11-bit identifier */
      /* has been received. Search rxArray form CANmodule for the same CAN-ID. */
      buffer = &CANmodule->rxArray[0];
      for (index = CANmodule->rxSize; index > 0U; index--)
      {
        if (((rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U)
        {
          msgMatched = true;
          break;
        }
        buffer++;
      }
    }
    /* Call specific function, which will process the message */
    if (msgMatched && (buffer != NULL) && (buffer->CANrx_callback != NULL))
    {
      buffer->CANrx_callback(buffer->object, &rcvMsg);
    }
  }
}

/******************************************************************************/
void CO_CANmodule_process(CO_CANmodule_t *CANmodule)
{
  // TODO - Need to fill this in
}