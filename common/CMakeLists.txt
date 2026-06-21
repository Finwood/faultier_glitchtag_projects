# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources(gpio.c uart.c)
zephyr_include_directories(.)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/export_compile_commands.cmake)
