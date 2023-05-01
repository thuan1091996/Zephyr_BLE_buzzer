# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE src/main.c)

# Custom files and folders
target_sources(app PRIVATE
    src/buzzer.c
    src/battery.c
    src/ble/bluetoothle.c
    src/ble/gatt_bas.c
    src/ble/gatt_custom.c
)
