#pragma once

#include <esp_log.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define APP_STORAGE_PARAM_CHECK(con) do { \
        if (!(con)) { \
            ESP_LOGE(TAG, "<ESP_QCLOUD_ERR_INVALID_ARG> !(%s)", #con); \
            return ESP_ERR_INVALID_ARG; \
        } \
    } while(0)

#define APP_STORAGE_ERROR_CHECK(con, err, format, ...) do { \
        if (con) { \
            if(*format != '\0') \
                ESP_LOGW(TAG, "<%s> " format, esp_err_to_name(err), ##__VA_ARGS__); \
            return err; \
        } \
    } while(0)

esp_err_t app_storage_init(void);

esp_err_t app_storage_set(const char *key, const void *value, size_t length);

esp_err_t app_storage_get(const char *key, void *value, size_t length);

esp_err_t app_storage_erase(const char *key);

#ifdef __cplusplus
}
#endif
