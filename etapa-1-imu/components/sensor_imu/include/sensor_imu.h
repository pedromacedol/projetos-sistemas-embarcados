#ifndef SENSOR_IMU_H
#define SENSOR_IMU_H

#include <math.h>
#include <esp_err.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
typedef struct
{
	float accel_X;
	float accel_Y;
	float accel_Z;
} AccelerationData;

typedef struct
{
	int16_t rotation_X;
	int16_t rotation_Y;
	int16_t rotation_Z;
} GyroscopeData;

/**
 * @brief Initializes the inertial measurement unit (IMU) sensor.
 *
 * This function initializes the IMU sensor by configuring the necessary parameters
 * and installing the driver for communication. It also verifies the sensor's identity
 * to ensure proper communication and functionality.
 *
 * @param devAddr Device address of the IMU sensor.
 * @param sda_pin GPIO number for the SDA (data) pin.
 * @param scl_pin GPIO number for the SCL (clock) pin.
 * @return
 * - ESP_OK: Success.
 * - ESP_ERR_NOT_FOUND: Initialization failure.
 */
esp_err_t imu_init(uint8_t devAddr, gpio_num_t sda_pin, gpio_num_t scl_pin);

/**
 * @brief Retrieves acceleration data from the sensor.
 *
 * This function retrieves acceleration data from the connected sensor and
 * populates the provided AccelerationData structure with the latest data.
 *
 * @param data Pointer to an AccelerationData structure to store the retrieved data.
 * @return
 * - ESP_OK: Success.
 * - ESP_FAIL: Initialization failure.
 */
esp_err_t imu_get_acceleration_data(AccelerationData *data);

/**
 * @brief Retrieves gyroscope data from the sensor.
 *
 * This function retrieves gyroscope data from the connected sensor and
 * populates the provided GyroscopeData structure with the latest data.
 *
 * @param data Pointer to a GyroscopeData structure to store the retrieved data.
 * @return
 * - ESP_OK: Success.
 * - ESP_FAIL: Initialization failure.
 */
esp_err_t imu_get_gyroscope_data(GyroscopeData *data);

/**
 * @brief Deinitializes the IMU and frees the associated resources.
 *
 * This function disables the IMU (Inertial Measurement Unit) communication and
 * releases any resources that were allocated during initialization.
 *
 * @return
 * - ESP_OK: Success.
 * - ESP_FAIL: Initialization failure.
 */
esp_err_t imu_deinit();

#endif