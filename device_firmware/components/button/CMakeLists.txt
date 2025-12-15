idf_component_register(SRCS "button_adc.c" "button_gpio.c" "iot_button.c"
                       INCLUDE_DIRS "include"
                                    "${IDF_PATH}/components/driver/deprecated"
                                    "${IDF_PATH}/components/esp_driver_gpio/include"
                                    "${IDF_PATH}/components/esp_adc/deprecated/include"
                                    "${IDF_PATH}/components/esp_timer/include")
                       # PRIV_REQUIRES esp_adc_cal)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=format -Wno-format)