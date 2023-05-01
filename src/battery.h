/****************************************************************************
* Title                 :    header file
* Filename              :   battery.h
* Author                :   thuantm5
* Origin Date           :   2023/05/01
* Version               :   v0.0.0
* Compiler              :   nRF connect SDK v2.3
* Target                :   nRF52
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2023/05/01    v0.0.0         	thuantm5      Interface Created.
*
*****************************************************************************/

/** \file battery.h
 *  \brief This module contains .
 *
 *  This is the header file for 
 */
#ifndef BATTERY_H_
#define BATTERY_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
// Voltage to level curve for a coin cell 3V battery. 
#define BATTERY_FULL_MV         2950
#define BATTERY_HALF_MV         2900
#define BATTERY_QUARTER_MV      2800
#define BATTERY_DEAD_MV         2300

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
uint8_t battery_level(void);
#endif // BATTERY_H_

/*** End of File **************************************************************/