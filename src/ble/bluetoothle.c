/*******************************************************************************
* Title                 :   Bluetooth LE 
* Filename              :   bluetoothle.c
* Author                :   Itachi
* Origin Date           :   2022/09/04
* Version               :   0.0.0
* Compiler              :   NCS toolchain v2.0.0
* Target                :   nRF52840dk
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2022/09/04       0.0.0	         Itachi      Module Created.
*
*******************************************************************************/

/** \file bluetoothle.c
 * \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "bluetoothle.h"
#include <zephyr/logging/log.h>
#include "gatt_bas.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MODULE_NAME			        ble
#define MODULE_LOG_LEVEL	        LOG_LEVEL_DBG
#define DEVICE_NAME		            CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN		        (sizeof(DEVICE_NAME) - 1) // Ignore null terminated
#define BT_KEYBOARD_APPEARANCE       0x03C1  /* More on: https://specificationrefs.bluetooth.com/assigned-values/Appearance%20Values.pdf */
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static const struct bt_data ADV_DATA[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    BT_DATA(BT_DATA_GAP_APPEARANCE, ((uint16_t []) {BT_KEYBOARD_APPEARANCE}), sizeof(uint16_t))
};


static struct bt_conn_cb ble_cb = {
	.connected 		= &on_connected_callback,
	.disconnected 	= &on_disconnected_callback,
};

struct bt_conn *current_conn;


/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ble_init_callback(int err)
{
    __ASSERT(0 == err, "Init BLE failed with error: %d", err);
    LOG_INF("BLE init succesfully");
}


int ble_init(void)
{
    int errorcode= 0;
    LOG_INF("BLE initializing \n\r");

    /* Assign callbacks for connection events */
    bt_conn_cb_register(&ble_cb);

    /* BLE initialization */
    errorcode = bt_enable(NULL);
    if(errorcode)
    {
        LOG_ERR("bt_enable return err %d \r\n", errorcode);
        return errorcode;
    }
    LOG_INF("BLE init succesfully");

    ble_adv_start();
    return 0;
}

void on_connected_callback(struct bt_conn *conn, uint8_t err)
{
	if(err) {
		LOG_ERR("BLE connection err: %d", err);
		return;
	}
	LOG_INF("BLE Connected.");
	current_conn = bt_conn_ref(conn);
}

void on_disconnected_callback(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("BLE Disconnected (reason: %d)", reason);
	if(current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}
    ble_adv_start();
}

int ble_adv_start(void)
{
	int errorcode = bt_le_adv_start(BT_LE_ADV_CONN, ADV_DATA, ARRAY_SIZE(ADV_DATA), NULL, 0);
    if (errorcode) {
        LOG_ERR("Couldn't start advertising (err = %d)", errorcode);
        return errorcode;
    }
    LOG_INF("Advertising successfully started\n");
    return 0;
}