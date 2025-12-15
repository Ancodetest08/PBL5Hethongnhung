#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "iot_button.h"
#include "light_driver.h"
#include "esp_log.h"
#include "app_priv.h"
#include <esp_rmaker_utils.h>
#include DEVELOPMENT_BOARD

#define TAG "app_driver"

#define REBOOT_DELAY    2

static bool g_output_state = true;

static void push_btn_cb(void *arg) {
    app_driver_set_state(!g_output_state);
}

static void factory_reset_trigger(void *arg) {
    esp_rmaker_factory_reset(0, REBOOT_DELAY);
}

void app_driver_init() {
    button_config_t btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num     = LIGHT_BUTTON_GPIO,
            .active_level = LIGHT_BUTTON_ACTIVE_LEVEL,
        },
    };
    button_handle_t btn_handle = iot_button_create(&btn_cfg);
    if (btn_handle) {
        iot_button_register_cb(btn_handle, BUTTON_SINGLE_CLICK, push_btn_cb);
        iot_button_register_cb(btn_handle, BUTTON_LONG_PRESS_START, factory_reset_trigger);
    }

    light_driver_config_t driver_config = {
        .gpio_red        = LIGHT_GPIO_RED,
        .gpio_green      = LIGHT_GPIO_GREEN,
        .gpio_blue       = LIGHT_GPIO_BLUE,
        .gpio_cold       = LIGHT_GPIO_COLD,
        .gpio_warm       = LIGHT_GPIO_WARM,
        .fade_period_ms  = LIGHT_FADE_PERIOD_MS,
        .blink_period_ms = LIGHT_BLINK_PERIOD_MS,
        .freq_hz         = LIGHT_FREQ_HZ,
        .clk_cfg         = LEDC_USE_APB_CLK,
        .duty_resolution = LEDC_TIMER_11_BIT,
    };

    ESP_ERROR_CHECK(light_driver_init(&driver_config));

    app_light_set_power(true);
}

int IRAM_ATTR app_driver_set_state(bool state) {
    if (g_output_state != state) {
        g_output_state = state;
        if (g_output_state) {
            ESP_LOGI(TAG, "Light ON");
            light_driver_set_switch(true);
        } else {
            ESP_LOGI(TAG, "Light OFF");
            light_driver_set_switch(false);
        }
    }

    return ESP_OK;
}

bool app_driver_get_state(void) {
    return g_output_state;
}

esp_err_t app_light_set_power(bool power) {
    if (power) {
        app_pm_lock_acquire();
        light_driver_set_switch(true);
    } else {
        light_driver_set_switch(false);
        app_pm_lock_release();
    }

    return ESP_OK;
}

esp_err_t app_light_set(uint32_t hue, uint32_t saturation, uint32_t brightness) {
    return light_driver_set_hsv(hue,saturation, brightness);
}

esp_err_t app_light_set_brightness(uint16_t brightness) {
    return light_driver_set_value(brightness);
}

esp_err_t app_light_set_hue(uint16_t hue) {
    return light_driver_set_hue(hue);
}

esp_err_t app_light_set_saturation(uint16_t saturation) {
    return light_driver_set_saturation(saturation);
}
