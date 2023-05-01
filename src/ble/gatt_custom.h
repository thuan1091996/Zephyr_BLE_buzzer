/****************************************************************************
* Title                 :    header file
* Filename              :   gatt_custom.h
* Author                :   thuantm96
* Origin Date           :   2023/03/17
* Version               :   v0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nrf52
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2023/03/17    v0.0.0         	thuantm96      Interface Created.
*
*****************************************************************************/

/** \file gatt_custom.h
 *  \brief This module contains .
 *
 *  This is the header file for 
 */
#ifndef BLE_GATT_CUSTOM_H_
#define BLE_GATT_CUSTOM_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "../buzzer.h"
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
/** @brief UUID of the Remote Service. **/

#define BT_UUID_CUSTOM_SERV_VAL \
	BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the Button Characteristic. **/
#define BT_UUID_CUSTOM_OUTPUT_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0002, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

#define BT_UUID_CUSTOM_SERVICE          BT_UUID_DECLARE_128(BT_UUID_CUSTOM_SERV_VAL)
#define BT_UUID_CUSTOM_OUTPUT_CHRC 	    BT_UUID_DECLARE_128(BT_UUID_CUSTOM_OUTPUT_CHRC_VAL)


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
ssize_t buzzer_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t buzzer_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t buzzer_cccd_changed(const struct bt_gatt_attr *attr, uint16_t value);


#endif // BLE_GATT_CUSTOM_H_

/*** End of File **************************************************************/