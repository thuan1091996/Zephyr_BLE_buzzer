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
#include "bluetoothle.h"

#include "../buzzer.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MODULE_NAME			        gatt_custom
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

static bool g_is_custom_notify_en = false;
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
ssize_t buzzer_read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    LOG_INF("Buzzer service read callback called\n");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, buzzer_state, sizeof(buzzer_state));
}

ssize_t buzzer_write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    uint8_t input_value = *((uint8_t*)buf);
    LOG_INF("Buzzer service write callback called, Input data: %d \n", input_value);
    if(input_value == 0)
    {
        buzzer_off();
    }
    else
    {
        buzzer_on();
    }
    return len;
}

void buzzer_cccd_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
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
    LOG_INF("Sensor notification %s.", g_is_custom_notify_en ? "enabled" : "disabled");
}
