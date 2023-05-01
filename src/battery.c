/*******************************************************************************
* Title                 :    
* Filename              :   battery.c
* Author                :   thuantm5
* Origin Date           :   2023/05/01
* Version               :   0.0.0
* Compiler              :   nRF connect SDK v2.3
* Target                :   nRF52
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/05/01       0.0.0	         thuantm5      Module Created.
*
*******************************************************************************/

/** \file battery.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "battery.h"
#include <zephyr/drivers/adc.h>
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MODULE_NAME			battery
#define MODULE_LOG_LEVEL	LOG_LEVEL_DBG
LOG_MODULE_REGISTER(MODULE_NAME, MODULE_LOG_LEVEL);

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,DT_SPEC_AND_COMMA)
};
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint16_t raw_adc_mv;
struct adc_sequence sequence = {
    .buffer = &raw_adc_mv,
    /* buffer size in bytes, not number of samples */
    .buffer_size = sizeof(raw_adc_mv),
};
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void battery_init()
{
    int err;
    LOG_INF("Battery init");
    if (!device_is_ready(adc_channels[0].dev)) {
		LOG_ERR("ADC controller device not ready, error: %d\n", err);
		return;
	}
	err = adc_channel_setup_dt(&adc_channels[0]);
	if (err < 0) {
		LOG_ERR("Could not setup channel #%d (%d)\n", 0, err);
		return;
	}
	adc_sequence_init_dt(&adc_channels[0], &sequence);
    LOG_INF("Battery init successfully \n");

}

uint8_t battery_level(void) {
	int err;
	int32_t val_mv;
    uint8_t batt_percent = 0; 

	err = adc_read(adc_channels[0].dev, &sequence);
	if (err < 0) {
		LOG_ERR("Could not read (%d)\n", err);
		return batt_percent; 
	}

	val_mv = raw_adc_mv;
	err = adc_raw_to_millivolts_dt(&adc_channels[0], &val_mv);
	if (err < 0) {
		LOG_ERR(" (value in mV not available)\n");
		return batt_percent; 
	}
    LOG_INF("Battery value: %d mV \n", val_mv);

	if (val_mv >= BATTERY_FULL_MV) 
    {
		batt_percent = 100; 
	} 
    else if (val_mv <= BATTERY_DEAD_MV) 
    {
		batt_percent = 0;
	} 
    else if (val_mv >= BATTERY_HALF_MV) 
    {
		batt_percent = (val_mv - BATTERY_HALF_MV) * 50 / (BATTERY_FULL_MV - BATTERY_HALF_MV) + 50;
	} 
    else if (val_mv >= BATTERY_QUARTER_MV)
    {
		batt_percent = (val_mv - BATTERY_QUARTER_MV) * 25 / (BATTERY_HALF_MV - BATTERY_QUARTER_MV) + 25;
	} 
    else 
    {
		batt_percent = (val_mv - BATTERY_DEAD_MV) * 25 / (BATTERY_QUARTER_MV - BATTERY_DEAD_MV) + 0;
	}
	return batt_percent; 
}
