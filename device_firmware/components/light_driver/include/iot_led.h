#ifndef __IOT_LED_H__
#define __IOT_LED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/ledc.h"

#define TIMER_BASE_CLK    80000000
#define LEDC_REF_CLK_HZ   1000000

#define HW_TIMER_GROUP (0)                                
#define HW_TIMER_ID (0)                                 
#define HW_TIMER_DIVIDER (16)                        
#define HW_TIMER_SCALE (TIMER_BASE_CLK / HW_TIMER_DIVIDER)
#define GAMMA_CORRECTION 0.8                            
#define GAMMA_TABLE_SIZE 256
#define DUTY_SET_CYCLE (20)

#define LIGHT_ERROR_CHECK(con, err, format, ...) do { \
        if (con) { \
            if(*format != '\0') \
                ESP_LOGW(TAG, "<%s> " format, esp_err_to_name(err), ##__VA_ARGS__); \
            return err; \
        } \
    } while(0)

#define LIGHT_PARAM_CHECK(con) do { \
        if (!(con)) { \
            ESP_LOGE(TAG, "<ESP_ERR_INVALID_ARG> !(%s)", #con); \
            return ESP_ERR_INVALID_ARG; \
        } \
    } while(0)

esp_err_t iot_led_init(ledc_timer_t timer_num, ledc_mode_t speed_mode, uint32_t freq_hz, ledc_clk_cfg_t clk_cfg, ledc_timer_bit_t duty_resolution);

esp_err_t iot_led_deinit();

esp_err_t iot_led_regist_channel(ledc_channel_t channel, gpio_num_t gpio_num);

esp_err_t iot_led_get_channel(ledc_channel_t channel, uint8_t* dst);

esp_err_t iot_led_set_channel(ledc_channel_t channel, uint8_t value, uint32_t fade_ms);

esp_err_t iot_led_start_blink(ledc_channel_t channel, uint8_t value, uint32_t period_ms, bool fade_flag);

esp_err_t iot_led_stop_blink(ledc_channel_t channel);

esp_err_t iot_led_set_gamma_table(const uint16_t gamma_table[GAMMA_TABLE_SIZE]);

#ifdef __cplusplus
}
#endif

#endif
