/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup ble_sdk_app_hids_keyboard_main main.c
 * @{
 * @ingroup ble_sdk_app_hids_keyboard
 * @brief HID Keyboard Sample Application main file.
 *
 * This file contains is the source code for a sample application using the HID, Battery and Device
 * Information Services for implementing a simple keyboard functionality.
 * Pressing Button 0 will send text 'hello' to the connected peer. On receiving output report,
 * it toggles the state of LED 2 on the mother board based on whether or not Caps Lock is on.
 * This application uses the @ref app_scheduler.
 *
 * Also it would accept pairing requests from any peer device.
 */

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_assert.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "bsp_btn_ble.h"
#include "app_scheduler.h"
#include "app_timer_appsh.h"
#include "fds.h"
#include "fstorage.h"

//#define NRF_LOG_MODULE_NAME "APP"
//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"

#include <buttons_leds.h>
#include <ble_stack.h>
#include <interconnect.h>
//#include <nrf52_uart2.h>

#if BUTTONS_NUMBER < 2
#error "Not enough resources on board"
#endif

#define DEAD_BEEF                        0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define SCHED_MAX_EVENT_DATA_SIZE        MAX(APP_TIMER_SCHED_EVT_SIZE, \
                                             BLE_STACK_HANDLER_SCHED_EVT_SIZE)       /**< Maximum size of scheduler events. */
#ifdef SVCALL_AS_NORMAL_FUNCTION
#define SCHED_QUEUE_SIZE                 20                                          /**< Maximum number of events in the scheduler queue. More is needed in case of Serialization. */
#else
#define SCHED_QUEUE_SIZE                 10                                          /**< Maximum number of events in the scheduler queue. */
#endif


//#define MAX_KEYS_IN_ONE_REPORT           (INPUT_REPORT_KEYS_MAX_LEN - SCAN_CODE_POS) /**< Maximum number of key presses that can be sent in one Input Report. */



/**@brief Function for the Event Scheduler initialization.
 */
static void scheduler_init(void)
{
   APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}


static void button_disconnect(uint8_t data)
{
   uint32_t       err_code;
   err_code = sd_ble_gap_disconnect(m_conn_handle,
                           BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
   if (err_code != NRF_ERROR_INVALID_STATE)
   {
      APP_ERROR_CHECK(err_code);
   }
}

static void button_whitelist_off(uint8_t data)
{
   uint32_t       err_code;
   if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
   {
      err_code = ble_advertising_restart_without_whitelist();
      if (err_code != NRF_ERROR_INVALID_STATE)
      {
         APP_ERROR_CHECK(err_code);
      }
   }
}

static void button_key_press(uint8_t data)
{
   if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
   {
      keys_send(1, &data);
   }
}

static void register_button_events()
{
   register_button_callback(BSP_EVENT_DISCONNECT, &button_disconnect);
   register_button_callback(BSP_EVENT_WHITELIST_OFF, &button_whitelist_off);
   register_button_callback(BSP_EVENT_KEY_0, &button_key_press);
}

enable_master_msg_t     enable_master_msg       = { {CmdCommand_SYN, 0x01, sizeof(enable_master_msg_t), CmdEnableMaster}, 0xFF, usb_protocol};
/**@brief Function for application main entry.
 */
int main(void)
{

   register_button_events();
   ble_stack_init();
   scheduler_init();

//   uint8_t tx_pin = 7;
//   nrf_gpio_cfg_output(tx_pin);
//   nrf_gpio_pin_clear(tx_pin);
//#define BUF_SIZE 32
//   uint8_t uart_rx_buf[BUF_SIZE] = {0};
//   uint8_t uart_tx_buf[BUF_SIZE] = {0};
//
//   uart_handle_t uart = uart_config(0, 1);
//   if (uart == NULL)
//      return 0;
//   uart_rx_dma_setup(0, uart, uart_rx_buf, BUF_SIZE);
//
//   uart_start(uart); 
//
//   // Enter main loop.
//   uint8_t data = 0xAA;
//   uint8_t last_rx_idx = 0;
//   uint8_t last_tx_idx = 0;
   Iconnect_setup(ICONNECT_ENABLE_DOWN);
   
   enable_master_msg.id = 1;
   enable_master_msg.output_protocol = usb_protocol;
   
   error_code_t err = NOT_READY;
   while (err != SUCCESS)
   {
      err = Iconnect_send_msg(Iconnect_down, &enable_master_msg.header);
      nrf_delay_ms(100); 
   }
    
   for (;;)
   {
//      uart_tx_buf[last_tx_idx] = data;
//      last_tx_idx = (last_tx_idx+1) % BUF_SIZE;
//      uart_send(uart, data++);
//      nrf_delay_us(200);
//      
//      uint8_t cur_rx_idx = uart_rx_dma_buffer_position(uart);
//         
//      while (last_rx_idx != cur_rx_idx)
//      {
//         if (uart_rx_buf[last_rx_idx] == uart_tx_buf[last_rx_idx])//last_data+1)
//            nrf_gpio_pin_clear(tx_pin);
//         else
//            nrf_gpio_pin_set(tx_pin);
//            
//         last_rx_idx = (last_rx_idx + 1) % BUF_SIZE;
//      }

      app_sched_execute();
   }
 
}


/**
 * @}
 */
