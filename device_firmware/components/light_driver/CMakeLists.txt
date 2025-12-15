idf_component_register(SRCS "light_driver.c" "iot_led.c" "./light_driver.c" "./iot_led.c"
                       INCLUDE_DIRS "."
                                    "./include"
                                    "${IDF_PATH}/components/driver/deprecated"
                                    "${IDF_PATH}/components/esp_driver_gpio/include"
                                    "${IDF_PATH}/components/esp_driver_ledc/include"
                       REQUIRES app_storage)
