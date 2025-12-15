#ifndef __IOT_BUTTON_ADC_H__
#define __IOT_BUTTON_ADC_H__

#include "driver/gpio.h"
#include "driver/adc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ADC_BUTTON_COMBINE(channel, index) ((channel)<<8 | (index))
#define ADC_BUTTON_SPLIT_INDEX(data) ((uint32_t)(data)&0xff)
#define ADC_BUTTON_SPLIT_CHANNEL(data) (((uint32_t)(data) >> 8) & 0xff)

typedef struct {
    adc1_channel_t adc_channel;
    uint8_t button_index;
    uint16_t min;
    uint16_t max;
} button_adc_config_t;

esp_err_t button_adc_init(const button_adc_config_t *config);

esp_err_t button_adc_deinit(adc1_channel_t channel, int button_index);

uint8_t button_adc_get_key_level(void *button_index);

#ifdef __cplusplus
}
#endif

#endif
