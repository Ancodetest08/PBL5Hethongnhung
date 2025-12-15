#pragma once

#include <esp_err.h>

typedef enum {
    POP_TYPE_MAC,
    POP_TYPE_RANDOM
} app_wifi_pop_type_t;

void app_wifi_init();

esp_err_t app_wifi_start(app_wifi_pop_type_t pop_type);
