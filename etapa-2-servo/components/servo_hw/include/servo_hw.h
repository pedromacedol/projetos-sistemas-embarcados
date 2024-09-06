#ifndef SERVO_HW_H
#define SERVO_HW_H

#include "esp_err.h"
#include <hal/ledc_types.h>

ledc_channel_t get_ledc_channel_gpio(uint8_t gpio_num);

/**
 * @brief Inicializa o servo motor e aloca os recursos.
 *
 * Habilita o PWM no GPIO especificado e aloca os recursos a serem utilizados.
 *
 * @return
 * - ESP_OK: Sucesso.
 * - ESP_FAIL: Erro de inicialização.
 */
esp_err_t hw_servo_init(uint8_t gpio_num);

/**
 * @brief Define a largura de pulso para o controle do ângulo do servo.
 *
 * @return
 * - ESP_OK: Sucesso.
 * - ESP_FAIL: Erro de inicialização.
 */
esp_err_t hw_servo_set_pulse_width(uint8_t gpio_num, uint32_t pulse_width_us);

/**
 * @brief Desinicializa o servo motor e libera os recursos.
 *
 * Desabilita o PWM no GPIO especificado e libera os recursos utilizados.
 *
 * @return
 * - ESP_OK: Sucesso.
 * - ESP_FAIL: Erro de inicialização.
 */
esp_err_t hw_servo_deinit(uint8_t gpio_num);

#endif // SERVO_HW_H