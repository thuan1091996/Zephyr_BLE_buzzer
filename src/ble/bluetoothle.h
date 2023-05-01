/****************************************************************************
* Title                 :   bluetooth le interface
* Filename              :   bluetoothle.h
* Author                :   Itachi
* Origin Date           :   2022/09/04
* Version               :   v0.0.0
* Compiler              :   NCS toolchain v2.0.0
* Target                :   nRF52840dk
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2022/09/04    v0.0.0         	Itachi      Interface Created.
*
*****************************************************************************/

/** \file bluetoothle.h
 *  \brief This module contains interface for ble with Zephyr RTOS using nRF52840dk
 *
 *  This is the header file for 
 */
#ifndef BLE_BLUETOOTHLE_H_
#define BLE_BLUETOOTHLE_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
int ble_init(void);
void on_connected_callback(struct bt_conn *conn, uint8_t err);
void on_disconnected_callback(struct bt_conn *conn, uint8_t reason);
int ble_adv_start(void);
struct bt_conn * ble_get_conn_info(void);
#endif // BLE_BLUETOOTHLE_H_

/*** End of File **************************************************************/