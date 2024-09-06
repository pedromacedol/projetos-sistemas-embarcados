#include "servo_hw.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "servo_tools.h"

typedef struct {
    uint8_t gpio_num;
    ledc_channel_t channel;
    bool in_use;
} ServoHWConfig;

#define MAX_SERVOS 8

ServoHWConfig servo_configs[MAX_SERVOS];

esp_err_t get_free_channel(uint8_t gpio_num, ledc_channel_t *channel) {
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (!servo_configs[i].in_use) {
            servo_configs[i].gpio_num = gpio_num;
            servo_configs[i].channel = (ledc_channel_t)i;
            printf("\nChannel: %d\n", servo_configs[i].channel);
            servo_configs[i].in_use = true;
            *channel = (ledc_channel_t)i;
            return ESP_OK;
        }
    }
    return ESP_ERR_NOT_FOUND;
}

ledc_channel_t get_ledc_channel_gpio(uint8_t gpio_num) {
    for (int i = 0; i < sizeof(servo_configs)/sizeof(servo_configs[0]); ++i) {
        if (servo_configs[i].gpio_num == gpio_num) {
            return servo_configs[i].channel;
        }
    }
    return LEDC_CHANNEL_MAX;
}

esp_err_t hw_servo_init(uint8_t gpio_num) {

    printf("\nIniciando servo no GPIO: %d", gpio_num);
    ledc_channel_t channel;
    esp_err_t err = get_free_channel(gpio_num, &channel);
    if (err != ESP_OK) return ESP_ERR_NOT_FOUND;


    // Configura o Timer do LEDC
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    esp_err_t ret = ledc_timer_config(&ledc_timer);
    if (ret != ESP_OK) return ESP_ERR_NOT_FOUND;

    // Configura o Channel do LEDC
    ledc_channel_config_t ledc_channel = {
        .gpio_num = gpio_num,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ret = ledc_channel_config(&ledc_channel);
    if (ret != ESP_OK) {
        printf("Erro ao configurar o canal LEDC para GPIO %d", gpio_num);
        return ESP_ERR_NOT_FOUND;
    }

    printf("\nInicio da varredura\n");
    // Vai de 0 a 180
    for (int angle = 0; angle <= 180; angle += 10) { 
        // Move o Servo para o angulo passado
        uint32_t duty_cycle = angle_to_duty(angle);
        hw_servo_set_pulse_width(gpio_num, duty_cycle);
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
    for (int angle = 180; angle >= 0; angle -= 10) { 
        uint32_t duty_cycle = angle_to_duty(angle);
        hw_servo_set_pulse_width(gpio_num, duty_cycle);
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
    printf("\nFim da varredura\n");

    return ESP_OK;

}


esp_err_t hw_servo_set_pulse_width(uint8_t gpio_num, uint32_t pulse_width_us) {

    int channel = get_ledc_channel_gpio(gpio_num);
    if (channel < 0) return ESP_FAIL; 

    esp_err_t ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, pulse_width_us);
    if(ret != ESP_OK) return ESP_FAIL;

    ret = ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
    if (ret != ESP_OK) return ESP_FAIL;
    
    return ESP_OK; 

}


esp_err_t hw_servo_deinit(uint8_t gpio_num) {
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servo_configs[i].gpio_num == gpio_num && servo_configs[i].in_use) {
            ledc_stop(LEDC_LOW_SPEED_MODE, servo_configs[i].channel, 0);
            servo_configs[i].in_use = false;
            return ESP_OK;
        }
    }
    return ESP_ERR_NOT_FOUND;
}