/****************************************************************************
* Title                 :    header file
* Filename              :   gatt_bas.h
* Author                :   thuantm5
* Origin Date           :   2023/03/16
* Version               :   v0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nrf52
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2023/03/16    v0.0.0         	thuantm5      Interface Created.
*
*****************************************************************************/

/** \file gatt_bas.h
 *  \brief This module contains .
 *
 *  This is the header file for 
 */
#ifndef BLE_GATT_BAS_H_
#define BLE_GATT_BAS_H_

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
ssize_t bas_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t bas_notify_cb(const struct bt_gatt_attr *attr, uint16_t value);

BT_GATT_SERVICE_DEFINE(BAS_SERVICE,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_BAS),
	BT_GATT_CHARACTERISTIC(BT_UUID_BAS_BATTERY_LEVEL,
			       		   BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       		   BT_GATT_PERM_READ, 
                   		   bas_read_cb, 
                   		   NULL,
			       		   NULL),
	BT_GATT_CCC(bas_notify_cb, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

#endif // BLE_GATT_BAS_H_

/*** End of File **************************************************************/