#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "servo_tools.h"
#include "esp_err.h"

#define SERVO_PIN_1 GPIO_NUM_13
#define SERVO_PIN_2 GPIO_NUM_12

typedef enum {
    INIT,
    CONFIG,
    ANGLE_SCAN,
    SET_ANGLE,
    GET_ANGLE,
    DEINIT,
    ERROR,
    IDLE
} ServoState;

void app_main() {
    ServoState current_state = INIT;
    esp_err_t ret1, ret2;
    ServoConfig servo1, servo2;
    ServoAngle angle1, angle2;
    ServoAngle new_angle1 = 0, new_angle2 = 60;

    while (1) {
        switch (current_state) {
            case INIT:
                printf("Estado: INIT\n");
                servo1 = (ServoConfig){ 
                    .gpio_num = SERVO_PIN_1, 
                    .pwm_freq = 50, .min_angle = 0, 
                    .max_angle = 180, 
                    .min_pulse_width = 1000, 
                    .max_pulse_width = 2000 };
                servo2 = (ServoConfig){ 
                    .gpio_num = SERVO_PIN_2, 
                    .pwm_freq = 50, 
                    .min_angle = 0, 
                    .max_angle = 180, 
                    .min_pulse_width = 1000, 
                    .max_pulse_width = 2000 };

                ret1 = servo_init(&servo1);
                ret2 = servo_init(&servo2);
                
                if (ret1 == ESP_OK && ret2 == ESP_OK) {
                    current_state = CONFIG;
                } else {
                    current_state = ERROR;
                }
                break;

            case CONFIG:
                printf("Estado: CONFIG\n");
                if (servo_init(&servo1) == ESP_OK && servo_init(&servo2) == ESP_OK) {
                    current_state = ANGLE_SCAN;
                } else {
                    current_state = ERROR;
                }
                break;

            case ANGLE_SCAN:
                printf("Estado: ANGLE_SCAN\n");
                current_state = SET_ANGLE;
                break;

            case SET_ANGLE:
                printf("Estado: SET_ANGLE\n");
                if (servo_set_angle(&servo1, new_angle1) == ESP_OK && servo_set_angle(&servo2, new_angle2) == ESP_OK) {
                    current_state = GET_ANGLE;
                } else {
                    current_state = ERROR;
                }
                vTaskDelay(1000 / portTICK_PERIOD_MS); 
                break;

            case GET_ANGLE:
                printf("Estado: GET_ANGLE\n");
                if (servo_get_angle(&servo1, &angle1) == ESP_OK && servo_get_angle(&servo2, &angle2) == ESP_OK) {
                    printf("Ângulo Servo 1: %d\n", angle1);
                    printf("Ângulo Servo 2: %d\n", angle2);
                    current_state = IDLE;
                } else {
                    current_state = ERROR;
                }
                break;

            case DEINIT:
                printf("Estado: DEINIT\n");
                // if (hw_servo_deinit(servo1->gpio_num) == ESP_OK) {
                //     printf("Servo 1 desinicializado.\n");
                //     current_state = IDLE;
                // } else {
                //     current_state = ERROR;
                // }
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                break;

            case ERROR:
                printf("Estado: ERROR\n");
                current_state = INIT; 
                break;

            case IDLE:
                printf("Estado: IDLE\n");
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                current_state = SET_ANGLE; 
                break;
        }
    }
}