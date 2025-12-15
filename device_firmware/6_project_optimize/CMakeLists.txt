cmake_minimum_required(VERSION 3.5)

add_compile_options(-fdiagnostics-color=always)

set(PROJECT_VER "3.0.0_optimized")

if(NOT DEFINED ENV{RAIMAKER_PATH})
    message(FATAL_ERROR "Please set RAIMAKER_PATH to esp-rainmaker repo")
endif(NOT DEFINED ENV{RAIMAKER_PATH})

set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/../components/light_driver
                         ${CMAKE_CURRENT_LIST_DIR}/../components/button
                         ${CMAKE_CURRENT_LIST_DIR}/../components/app_storage
                         ${CMAKE_CURRENT_LIST_DIR}/../components/app_wifi
                         $ENV{RAIMAKER_PATH}/components/esp-rainmaker/components/esp_rainmaker
                         $ENV{RAIMAKER_PATH}/components/esp-rainmaker/components/esp_schedule
                         $ENV{RAIMAKER_PATH}/components/json_generator
                         $ENV{RAIMAKER_PATH}/components/json_parser
                         # $ENV{RAIMAKER_PATH}/components/qrcode
                         $ENV{RAIMAKER_PATH}/components/esp-rainmaker/components/rmaker_common)

set(EXCLUDE_COMPONENTS "coap" "asio")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(6_performance_optimize)
