#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app_storage.h"
#include "esp_log.h"
#include "app_priv.h"

static const char *TAG = "light_drivers";

void app_main() {
    int i = 0;

    ESP_LOGE(TAG, "app_main");

    ESP_LOGI(TAG, "NVS Flash initialization");
    app_storage_init();

    ESP_LOGI(TAG, "Application driver initialization");
    app_driver_init();

    while (1) {
        ESP_LOGI(TAG, "[%02d] Hello world!", i++);
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
