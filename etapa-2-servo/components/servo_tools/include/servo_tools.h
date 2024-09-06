#ifndef SERVO_TOOLS_H
#define SERVO_TOOLS_H

#include "servo_hw.h"
#include "esp_err.h"

// Definição do tipo ServoAngle
typedef uint16_t ServoAngle;

typedef struct {
    uint8_t gpio_num;               // Pino GPIO conectado ao servo
    uint32_t pwm_freq;              // Frequência PWM
    ServoAngle min_angle;           // Ângulo mínimo permitido
    ServoAngle max_angle;           // Ângulo máximo permitido
    uint32_t min_pulse_width;       // Largura mínima do pulso (em microssegundos)
    uint32_t max_pulse_width;       // Largura máxima do pulso (em microssegundos)
} ServoConfig;

// Declaração das funções da biblioteca
esp_err_t servo_init(ServoConfig *config);
esp_err_t servo_set_angle(ServoConfig *config, ServoAngle angle);
esp_err_t servo_get_angle(const ServoConfig *config, ServoAngle *angle);

uint32_t angle_to_duty(uint32_t angle);

#endif // SERVO_TOOLS_H