cmake_minimum_required(VERSION 3.5)

add_compile_options(-fdiagnostics-color=always)

set(PROJECT_VER "2.0.0_UpdateFirmware")

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

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(5_rainmaker)
