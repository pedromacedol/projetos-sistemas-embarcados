#include "imu_tools.h"
#include <math.h>

esp_err_t imu_read_data(IMUData *data)
{
	if (data == NULL)
	{
		return ESP_FAIL;
	}

	esp_err_t result;

	result = imu_get_acceleration_data(&data->accelData);
	switch (result)
	{
	case ESP_OK:
		break;
	default:
		printf("Failed to get acceleration data: %s\n", esp_err_to_name(result));
		return result;
	}

	result = imu_get_gyroscope_data(&data->gyroData);
	switch (result)
	{
	case ESP_OK:
		break;
	default:
		printf("Failed to get gyroscope data: %s\n", esp_err_to_name(result));
		return result;
	}

	return ESP_OK;
}

esp_err_t imu_calculate_quaternion(const IMUData *data, Quaternion *quaternion)
{
	if (data == NULL || quaternion == NULL)
	{
		printf("Invalid input for quaternion calculation\n");
		return ESP_FAIL;
	}

	// Normalize accelerometer data
	float ax = data->accelData.accel_X;
	float ay = data->accelData.accel_Y;
	float az = data->accelData.accel_Z;

	// Normalize gyroscope data
	int gx = data->gyroData.rotation_X;
	int gy = data->gyroData.rotation_Y;
	int gz = data->gyroData.rotation_Z;

	// Time step (you need to define 'dt' based on your sensor update rate)
	float dt = 0.01; // Example: 10 ms update rate

	// Calculate the angles from the accelerometer
	float pitch = atan2(-ax, sqrt(ay * ay + az * az));
	float roll = atan2(ay, az);
	float yaw = 0; // Initial assumption, will be updated by integrating gyro data

	// Integrate gyroscope data
	pitch += gx * dt;
	roll += gy * dt;
	yaw += gz * dt;

	// Convert pitch, roll, yaw to quaternion
	float cy = cos(yaw * 0.5);
	float sy = sin(yaw * 0.5);
	float cp = cos(pitch * 0.5);
	float sp = sin(pitch * 0.5);
	float cr = cos(roll * 0.5);
	float sr = sin(roll * 0.5);

	quaternion->w = cr * cp * cy + sr * sp * sy;
	quaternion->x = sr * cp * cy - cr * sp * sy;
	quaternion->y = cr * sp * cy + sr * cp * sy;
	quaternion->z = cr * cp * sy - sr * sp * cy;

	return ESP_OK;
}

esp_err_t imu_calculate_euler_angles(const Quaternion *quaternion, EulerAngle *euler)
{
	if (quaternion == NULL || euler == NULL)
	{
		printf("Invalid input for Euler angle calculation\n");
		return ESP_FAIL;
	}

	// recebendo os valores do quaternion
	float qw = quaternion->w;
	float qx = quaternion->x;
	float qy = quaternion->y;
	float qz = quaternion->z;

	// armazendo no euler angles
	euler->roll = atan2(2 * (qw * qx + qy * qz), 1 - 2 * (qx * qx + qy * qy));
	euler->pitch = asin(2 * (qw * qy - qz * qx));
	euler->yaw = atan2(2 * (qw * qz + qx * qy), 1 - 2 * (qy * qy + qz * qz));

	return ESP_OK;
}