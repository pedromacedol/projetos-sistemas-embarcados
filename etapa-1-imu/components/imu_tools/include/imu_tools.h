#ifndef IMU_TOOLS_H
#define IMU_TOOLS_H

#include <esp_err.h>
#include "sensor_imu.h"

typedef struct
{
	AccelerationData accelData;
	GyroscopeData gyroData;
} IMUData;

typedef struct
{
	float w;
	float x;
	float y;
	float z;
} Quaternion;

typedef struct
{
	float roll;
	float pitch;
	float yaw;
} EulerAngle;

/**
 * @brief  Retrieves IMU (Inertial Measurement Unit) data from the sensor.
 *
 * This function retrieves IMU data, including both acceleration and gyroscope data,
 * from the connected sensor and populates the provided IMUData structure with the latest data.
 *
 * @param data Pointer to the IMUData struct containing accelerometer and gyroscope data.
 * @return
 * - ESP_OK: Success
 * - ESP_FAIL: Initialization failure.
 */
esp_err_t imu_read_data(IMUData *data);

/**
 * @brief Calculates quaternion data from IMU (Inertial Measurement Unit) data.
 *
 * This function calculates quaternion data based on the provided IMU data, which includes
 * both acceleration and gyroscope data. The resulting quaternion is stored in the provided
 * Quaternion structure.
 *
 * @param data Pointer to the IMUData struct
 * @param quaternion Pointer to the Quaternion struct
 * @return
 * - ESP_OK Success
 * - ESP_FAIL: Initialization failure.
 */
esp_err_t imu_calculate_quaternion(const IMUData *data, Quaternion *quaternion);

/**
 * @brief Converts the quaternion to Euler angles and stores them in the EulerAngle structure.
 *
 * This function converts a quaternion to Euler angles and stores the resulting angles
 * in the provided EulerAngle structure.
 *
 * @param quaternion Pointer to the Quaternion struct
 * @param euler Pointer to the EulerAngle struct
 * @return
 * - ESP_OK: Success
 * - ESP_FAIL: Initialization failure.
 */
esp_err_t imu_calculate_euler_angles(const Quaternion *quaternion, EulerAngle *euler);

#endif