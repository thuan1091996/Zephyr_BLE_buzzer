/*******************************************************************************
* Title                 :   
* Filename              :   buzzer.c
* Author                :   thuantm96
* Origin Date           :   2023/05/01
* Version               :   0.0.0
* Compiler              :   nRF connect SDK v2.3
* Target                :   nRF52
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/05/01       0.0.0	         thuantm96      Module Created.
*
*******************************************************************************/

/** \file buzzer.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "buzzer.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MODULE_NAME			buzzer
#define MODULE_LOG_LEVEL	LOG_LEVEL_INF
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
static struct gpio_dt_spec buzzer_dt = GPIO_DT_SPEC_GET(DT_NODELABEL(buzzer), gpios);


uint8_t buzzer_state = 0;
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void buzzer_init()
{
    int ret;
	if (!device_is_ready(buzzer_dt.port)) {
		LOG_ERR("Buzzer pin device not ready");
	}

	ret = gpio_pin_configure_dt(&buzzer_dt, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		LOG_ERR("Failed to config Buzzer pin with err: %d", ret);
	}
    buzzer_state = 0;
	LOG_INF("Buzzer pin init succesfully");
}

void buzzer_on()
{
    int ret;
    ret = gpio_pin_set_dt(&buzzer_dt, 1);
    if (ret < 0) {
        LOG_ERR("Failed to turn on Buzzer pin with err: %d", ret);
        return;
    }
    buzzer_state = 1;
}

void buzzer_off()
{
    int ret;
    ret = gpio_pin_set_dt(&buzzer_dt, 0);
    if (ret < 0) {
        LOG_ERR("Failed to turn off Buzzer pin with err: %d", ret);
        return;
    }
    buzzer_state = 0;

}