#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "app_storage.h"
#include "nvs.h"
#include "nvs_flash.h"

static const char *TAG = "app_storage";

esp_err_t app_storage_init() {
    static bool init_flag = false;
    if (!init_flag) {
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);

        init_flag = true;
    }

    return ESP_OK;
}

esp_err_t app_storage_erase(const char *key) {
    APP_STORAGE_PARAM_CHECK(key);

    esp_err_t ret     = ESP_OK;
    nvs_handle handle = 0;

    ret = nvs_open(CONFIG_RAINMAKER_APP_PARTITION_NAMESPACE, NVS_READWRITE, &handle);
    APP_STORAGE_ERROR_CHECK(ret != ESP_OK, ret, "Open non-volatile storage");

    if (!strcmp(key, CONFIG_RAINMAKER_APP_PARTITION_NAMESPACE)) {
        ret = nvs_erase_all(handle);
    } else {
        ret = nvs_erase_key(handle, key);
    }

    nvs_commit(handle);
    nvs_close(handle);

    APP_STORAGE_ERROR_CHECK(ret != ESP_OK && ret != ESP_ERR_NVS_NOT_FOUND, ret, "Erase key-value pair, key: %s", key);

    return ESP_OK;
}

esp_err_t app_storage_set(const char *key, const void *value, size_t length) {
    APP_STORAGE_PARAM_CHECK(key);
    APP_STORAGE_PARAM_CHECK(value);
    APP_STORAGE_PARAM_CHECK(length > 0);

    esp_err_t ret     = ESP_OK;
    nvs_handle handle = 0;

    ret = nvs_open(CONFIG_RAINMAKER_APP_PARTITION_NAMESPACE, NVS_READWRITE, &handle);
    APP_STORAGE_ERROR_CHECK(ret != ESP_OK, ret, "Open non-volatile storage");

    ret = nvs_set_blob(handle, key, value, length);

    nvs_commit(handle);
    nvs_close(handle);

    APP_STORAGE_ERROR_CHECK(ret != ESP_OK, ret, "Set value for given key, key: %s", key);

    return ESP_OK;
}

esp_err_t app_storage_get(const char *key, void *value, size_t length) {
    APP_STORAGE_PARAM_CHECK(key);
    APP_STORAGE_PARAM_CHECK(value);
    APP_STORAGE_PARAM_CHECK(length > 0);

    esp_err_t ret     = ESP_OK;
    nvs_handle handle = 0;

    ret = nvs_open(CONFIG_RAINMAKER_APP_PARTITION_NAMESPACE, NVS_READWRITE, &handle);
    APP_STORAGE_ERROR_CHECK(ret != ESP_OK, ret, "Open non-volatile storage");

    ret = nvs_get_blob(handle, key, value, &length);

    nvs_close(handle);

    if (ret == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGD(TAG, "<ESP_ERR_NVS_NOT_FOUND> Get value for given key, key: %s", key);
        return ESP_ERR_NVS_NOT_FOUND;
    }

    APP_STORAGE_ERROR_CHECK(ret != ESP_OK, ret, "Get value for given key, key: %s", key);

    return ESP_OK;
}
