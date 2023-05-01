/*******************************************************************************
* Title                 :    
* Filename              :   gatt_bas.c
* Author                :   thuantm96
* Origin Date           :   2023/03/16
* Version               :   0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nrf52
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/03/16       0.0.0	         thuantm96      Module Created.
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
#include "../battery.h"


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void batt_measure_cb(struct k_work *work);
void batt_measure_timeout(struct k_timer *dummy);


/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MODULE_NAME			        gatt_bas
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);



K_TIMER_DEFINE(batt_measure_timer, batt_measure_timeout, NULL);
K_WORK_DEFINE(batt_measure_work, batt_measure_cb);
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
bool g_is_bas_notify_en = false; //Use to allow send notification when char update
static const struct bt_gatt_attr* p_batt_level_attr; /* handle of either the characteristic or characteristic value attribute */
/**************************	****************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void batt_measure_cb(struct k_work *work)
{
    int ret;
    if (NULL == ble_get_conn_info())
	{
		LOG_ERR("No connection");
		return;
	}
	uint8_t cur_batt_level = battery_level();
    ret = bt_gatt_notify(ble_get_conn_info(), p_batt_level_attr, &cur_batt_level, sizeof(cur_batt_level));
    if (ret)
    {
        LOG_ERR("Notify failed (err %d)", ret);
    }
}

void batt_measure_timeout(struct k_timer *dummy)
{
    if(g_is_bas_notify_en)
    {
        k_work_submit(&batt_measure_work);
    }
};



// attribute read callback
ssize_t bas_read_cb(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{
	uint8_t cur_batt = battery_level();
	ssize_t ret_val= bt_gatt_attr_read(conn, attr, buf, len, offset, &cur_batt, sizeof(cur_batt));
	LOG_INF("Reading battery: %d", cur_batt);
	return ret_val;
}

// notification changed callback, log it
ssize_t bas_notify_cb(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);
	static bool first_time = true;
	if(first_time)
	{	
		p_batt_level_attr = bt_gatt_find_by_uuid(NULL, 0 , BT_UUID_BAS_BATTERY_LEVEL);
		if (NULL == p_batt_level_attr)
		{
			LOG_ERR("Cannot find custom characteristic handle");
		}
		else
		{
			LOG_INF("Found custom characteristic handle %p", p_batt_level_attr);
		}

		first_time=false;
	}
	g_is_bas_notify_en = (value == BT_GATT_CCC_NOTIFY);
	if(g_is_bas_notify_en)
	{
		k_timer_start(&batt_measure_timer, K_MSEC(1000), K_MSEC(1000));
	}
	else
	{
		k_timer_stop(&batt_measure_timer);
	}
	LOG_INF("Battery level notification %s.", g_is_bas_notify_en ? "enabled" : "disabled");
	return 0;
}
