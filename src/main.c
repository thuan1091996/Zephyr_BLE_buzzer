/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "ble/bluetoothle.h"
#include "buzzer.h"
#include "battery.h"

void main(void)
{
	int err;
	printk("Hello World! %s\n", CONFIG_BOARD);
	battery_init();
	buzzer_init();

	ble_init();	

	err = smp_bt_register();

	if (err) {
		printk("SMP BT register failed (err: %d)", err);
	}
}
