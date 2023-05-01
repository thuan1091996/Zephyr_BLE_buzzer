/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "ble/bluetoothle.h"
#include "buzzer.h"

void main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD);
	buzzer_init();
	ble_init();
}
