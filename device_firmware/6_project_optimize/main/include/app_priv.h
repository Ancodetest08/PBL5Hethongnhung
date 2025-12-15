#ifndef __APP_PRIVATE_H__
#define __APP_PRIVATE_H__

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

#define DEFAULT_POWER       true
#define DEFAULT_HUE         180
#define DEFAULT_SATURATION  100
#define DEFAULT_BRIGHTNESS  25

void app_driver_init(void);

int app_driver_set_state(bool state);

bool app_driver_get_state(void);

esp_err_t app_light_set(uint32_t hue, uint32_t saturation, uint32_t brightness);

esp_err_t app_light_set_power(bool power);

esp_err_t app_light_set_brightness(uint16_t brightness);

esp_err_t app_light_set_hue(uint16_t hue);

esp_err_t app_light_set_saturation(uint16_t saturation);

esp_err_t app_pm_init();

esp_err_t app_pm_lock_acquire();

esp_err_t app_pm_lock_release();

#endif
