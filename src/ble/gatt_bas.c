/*******************************************************************************
* Title                 :    
* Filename              :   gatt_bas.c
* Author                :   thuantm5
* Origin Date           :   2023/03/16
* Version               :   0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nrf52
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/03/16       0.0.0	         thuantm5      Module Created.
*
*******************************************************************************/

/** \file gatt_bas.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "bluetoothle.h"
#include <zephyr/logging/log.h>
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MODULE_NAME			        gatt_bas
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
uint8_t batt_level = 100;
static bool g_is_bas_notify_en = false; //Use to allow send notification when char update
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

// attribute read callback
ssize_t bas_read_cb(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{
	uint8_t cur_batt = batt_level;
	ssize_t ret_val= bt_gatt_attr_read(conn, attr, buf, len, offset, &cur_batt, sizeof(cur_batt));
	LOG_INF("Reading battery: %d", cur_batt);
	return ret_val;
}

// notification changed callback, log it
ssize_t bas_notify_cb(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);
	g_is_bas_notify_en = (value == BT_GATT_CCC_NOTIFY);
	LOG_INF("Battery level notification %s.", g_is_bas_notify_en ? "enabled" : "disabled");
	return 0;
}
