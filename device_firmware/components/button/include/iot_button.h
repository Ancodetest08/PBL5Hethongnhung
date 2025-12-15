#ifndef __IOT_BUTTON_H__
#define __IOT_BUTTON_H__

#include "button_gpio.h"
#include "button_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (* button_cb_t)(void *);
typedef void *button_handle_t;

typedef enum {
    BUTTON_PRESS_DOWN = 0,
    BUTTON_PRESS_UP,
    BUTTON_PRESS_REPEAT,
    BUTTON_SINGLE_CLICK,
    BUTTON_DOUBLE_CLICK,
    BUTTON_LONG_PRESS_START,
    BUTTON_LONG_PRESS_HOLD,
    BUTTON_EVENT_MAX,
    BUTTON_NONE_PRESS,
} button_event_t;

typedef enum {
    BUTTON_TYPE_GPIO,
    BUTTON_TYPE_ADC,
} button_type_t;

typedef struct {
    button_type_t type;
    union {
        button_gpio_config_t gpio_button_config;
        button_adc_config_t adc_button_config;
    };
} button_config_t;

button_handle_t iot_button_create(const button_config_t *config);

esp_err_t iot_button_delete(button_handle_t btn_handle);

esp_err_t iot_button_register_cb(button_handle_t btn_handle, button_event_t event, button_cb_t cb);

esp_err_t iot_button_unregister_cb(button_handle_t btn_handle, button_event_t event);

button_event_t iot_button_get_event(button_handle_t btn_handle);

uint8_t iot_button_get_repeat(button_handle_t btn_handle);

#ifdef __cplusplus
}
#endif

#endif
