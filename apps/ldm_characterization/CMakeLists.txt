# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(echo_bot)
target_include_directories(app PRIVATE ../../common/)
target_sources(app PRIVATE src/ldm_characterization.c ../../common/gpio.c ../../common/uart.c)
