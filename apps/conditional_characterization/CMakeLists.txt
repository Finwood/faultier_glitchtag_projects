# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED)
project(app)
target_sources(app PRIVATE src/conditional_characterization.c)
