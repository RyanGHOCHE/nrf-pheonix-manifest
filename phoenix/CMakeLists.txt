#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(phoenix LANGUAGES C VERSION 1.0.0)
FILE(GLOB app_sources src/*.c)
# configure_file(app_version.h.in ${CMAKE_BINARY_DIR}/app/include/app_version.h)
# target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR}/app/inc src)
target_include_directories(app PRIVATE inc)
target_sources(app PRIVATE ${app_sources})
