#include <stdio.h>
#include "imu_tools.h"
#include "sensor_imu.h"
#include "math.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define MPU6050_ADDR 0x68	// Endereço do sensor MPU6050
#define SDA_PIN GPIO_NUM_21 // Pino GPIO para SDA
#define SCL_PIN GPIO_NUM_22 // Pino GPIO para SCL

void app_main()
{
    esp_err_t result_imu;
    do {
        result_imu = imu_init(MPU6050_ADDR, SDA_PIN, SCL_PIN);
        if (result_imu != ESP_OK)
        {
            printf("Failed to init IMU Sensor: %s\n", esp_err_to_name(result_imu));
            vTaskDelay(pdMS_TO_TICKS(1000)); // Espera um segundo antes de tentar novamente
        }
    } while (result_imu != ESP_OK);
 
    while (1)
    {
        IMUData imu;
        esp_err_t result_imuData = imu_read_data(&imu);
        if (result_imuData != ESP_OK)
        {
            printf("Failed to get IMU data: %s\n", esp_err_to_name(result_imuData));
            do {
                result_imu = imu_init(MPU6050_ADDR, SDA_PIN, SCL_PIN);
                if (result_imu != ESP_OK)
                {
                    printf("Failed to re-init IMU Sensor: %s\n", esp_err_to_name(result_imu));
                    vTaskDelay(pdMS_TO_TICKS(1000)); // Espera um segundo antes de tentar novamente
                }
            } while (result_imu != ESP_OK);
        }
        else
        {
            printf(" \nIMU Data:\n\n- Aceleração: X = %.2fg, Y = %.2fg, Z = %.2fg \n- Giroscópio: X: %d°/sec, Y: %d°/sec, Z: %d°/sec\n",
                   imu.accelData.accel_X,  
                   imu.accelData.accel_Y,
                   imu.accelData.accel_Z,
                   imu.gyroData.rotation_X,
                   imu.gyroData.rotation_Y,
                   imu.gyroData.rotation_Z);

            // Calcular quaternions
            Quaternion quaternion;
            esp_err_t result_quaternion = imu_calculate_quaternion(&imu, &quaternion);
            if (result_quaternion != ESP_OK)
            {
                printf("Failed to calculate quaternion: %s\n", esp_err_to_name(result_quaternion));
            }
            else
            {
                printf("\nQuaternion: (w: %.3f, x: %.3f, y: %.3f, z: %.3f)\n",
                       quaternion.w, quaternion.x, quaternion.y, quaternion.z);
            }

            // Calcular euler angles
            EulerAngle euler;
            esp_err_t result_euler = imu_calculate_euler_angles(&quaternion, &euler);
            if (result_euler != ESP_OK)
            {
                printf("Failed to calculate euler: %s\n", esp_err_to_name(result_euler));
            }
            else
            {
                printf("\nEuler: (roll: %.3f, pitch: %.3f, yaw: %.3f)\n",
                       euler.roll, euler.pitch, euler.yaw);
            }

            printf("\n--------------\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}