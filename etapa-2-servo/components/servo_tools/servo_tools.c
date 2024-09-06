#include "servo_tools.h"
#include "driver/ledc.h"

uint32_t angle_to_duty(uint32_t angle) {
    const int minDuty = (int)(0.023 * 65536);
    const int maxDuty = (int)(0.124 * 65536);
    return minDuty + (angle * (maxDuty - minDuty) / 180);
}

uint32_t duty_to_angle(uint32_t duty_cycle) {
    const int minDuty = (int)(0.023 * 65536);  // Duty cycle mínimo para 0 graus
    const int maxDuty = (int)(0.124 * 65536);  // Duty cycle máximo para 180 graus

    // Verifica se o duty_cycle está dentro dos limites aceitáveis
    if (duty_cycle < minDuty) {
        return 0;
    } else if (duty_cycle > maxDuty) {
        return 180;
    }
    uint32_t angle = (duty_cycle - minDuty) * 180 / (maxDuty - minDuty);
    return angle;
}


esp_err_t servo_init(ServoConfig *config) {
    if (config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Inicializa o servo na porta passada no config
    esp_err_t err = hw_servo_init(config->gpio_num);

    if (err != ESP_OK) return err;

    return ESP_OK;
}

esp_err_t servo_set_angle(ServoConfig *config, ServoAngle angle) {
    if (config == NULL || angle < config->min_angle || angle > config->max_angle) {
        return ESP_ERR_INVALID_ARG;
    }

    // Converte o ângulo para ciclo de trabalho correspondente
    uint32_t duty_cycle = angle_to_duty(angle);

    return hw_servo_set_pulse_width(config->gpio_num, duty_cycle);

}

esp_err_t servo_get_angle(const ServoConfig *config, ServoAngle *angle) {
    if (config == NULL || angle == NULL) return ESP_ERR_INVALID_ARG;

    ledc_channel_t channel = get_ledc_channel_gpio(config->gpio_num);
    if (channel == LEDC_CHANNEL_MAX) return ESP_ERR_INVALID_ARG; 

    // Converte o ciclo de trabalho para ângulo correspondente
    uint32_t duty_cycle = ledc_get_duty(LEDC_LOW_SPEED_MODE, channel);
    printf("Canal: %d\n", channel);
    printf("Duty cycle lido para GPIO %d: %ld\n", config->gpio_num, duty_cycle);

    *angle = ((duty_cycle - (config->min_pulse_width * (65536 / (config->pwm_freq * 1000)))) * (config->max_angle - config->min_angle)) /
              ((config->max_pulse_width * (65536 / (config->pwm_freq * 1000))) - (config->min_pulse_width * (65536 / (config->pwm_freq * 1000)))) + config->min_angle;


    return ESP_OK;
}