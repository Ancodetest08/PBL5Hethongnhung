#ifndef __IOT_BUTTON_GPIO_H__
#define __IOT_BUTTON_GPIO_H__

#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t gpio_num;
    uint8_t active_level;
} button_gpio_config_t;

esp_err_t button_gpio_init(const button_gpio_config_t *config);

esp_err_t button_gpio_deinit(int gpio_num);

uint8_t button_gpio_get_key_level(void *gpio_num);

#ifdef __cplusplus
}
#endif

#endif
