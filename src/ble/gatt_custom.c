/*******************************************************************************
* Title                 :    
* Filename              :   gatt_custom.c
* Author                :   thuantm96
* Origin Date           :   2023/03/17
* Version               :   0.0.0
* Compiler              :   nRF connect SDK 2.3
* Target                :   nrf52
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/03/17       0.0.0	         thuantm96      Module Created.
*
*******************************************************************************/

/** \file gatt_custom.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include "gatt_custom.h"

#include "bluetoothle.h"
#include "../buzzer.h"

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void buzzer_off_work_cb(struct k_work *work);
void buzzer_timer_timeout(struct k_timer *dummy);

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MODULE_NAME			        gatt_custom
#define MODULE_LOG_LEVEL	        LOG_LEVEL_INF
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);


// Custom service definition
BT_GATT_SERVICE_DEFINE(BUZZER_SERVICE,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_OUTPUT_CHRC,
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY | BT_GATT_CHRC_WRITE,
                           BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                           &buzzer_read_cb,
                           &buzzer_write_cb,
                           NULL),
    BT_GATT_CCC(buzzer_cccd_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

K_TIMER_DEFINE(buzzer_off_timer, buzzer_timer_timeout, NULL);
K_WORK_DEFINE(buzzer_off_work, buzzer_off_work_cb);
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static const struct bt_gatt_attr* p_custom_attr; /* handle of either the characteristic or characteristic value attribute */
static bool g_is_custom_notify_en = false;

/******************************************************************************
* Function Definitions
*******************************************************************************/
void buzzer_off_work_cb(struct k_work *work)
{
    int ret;
    buzzer_off();
    if (NULL == ble_get_conn_info())
	{
		LOG_ERR("No connection");
		return;
	}
    ret = bt_gatt_notify(ble_get_conn_info(), p_custom_attr, &buzzer_state, sizeof(buzzer_state));
    if (ret)
    {
        LOG_ERR("Notify failed (err %d)", ret);
    }
}

void buzzer_timer_timeout(struct k_timer *dummy)
{
    LOG_INF("Buzzer timer timeout\n");
    if(g_is_custom_notify_en)
    {
        k_work_submit(&buzzer_off_work);
    }
};

ssize_t buzzer_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    LOG_INF("Buzzer service read callback called, current state %d \n", buzzer_state);
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &buzzer_state, sizeof(buzzer_state));
}

ssize_t buzzer_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    uint8_t input_value = *((uint8_t*)buf);
    LOG_INF("Buzzer service write callback called, Input data: %d \n", input_value);
    if(input_value == 0)
    {
        buzzer_off();
        k_timer_stop(&buzzer_off_timer);
    }
    else
    {
        buzzer_on();
        // Start timer to turn off buzzer
        k_timer_start(&buzzer_off_timer, K_MSEC(BUZZER_OFF_TIMEOUT), K_NO_WAIT);
    }
    return len;
}

ssize_t buzzer_cccd_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);

    //Find the attribute of CHAR or CHAR value in order to notify
    static bool first_time = true;
	if(first_time)
	{
		p_custom_attr = bt_gatt_find_by_uuid(NULL, 0 , BT_UUID_CUSTOM_OUTPUT_CHRC);
		if (NULL == p_custom_attr)
		{
			LOG_ERR("Cannot find buzzer characteristic handle");
		}
		else
		{
			LOG_INF("Found buzzer characteristic handle %p", p_custom_attr);
		}
		first_time=false;
	}

    switch(value)
    {
        case BT_GATT_CCC_NOTIFY: 
            g_is_custom_notify_en = true;
            break;

        case 0: 
            g_is_custom_notify_en = false;
            break;
        
        default: 
            printk("Error, CCCD has been set to an invalid value");     
    }
    LOG_INF("Buzzer notification %s.", g_is_custom_notify_en ? "enabled" : "disabled");
}
