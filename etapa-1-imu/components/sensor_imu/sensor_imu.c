#include "sensor_imu.h"
#include "driver/i2c.h"

// Define the register address of the accelerometer
#define ACCEL_REG_ADDR 0x3B

// Define a scaling factor for acceleration values.
#define ACCEL_SCALE_FACTOR 16384.0f

// Define the register address of the gyroscope
#define GYRO_REG_ADDR 0x43

// Define the scaling factor for converting gyroscope digital values to physical units (°/s).
// 250.0f: Maximum measurable angular rate in degrees per second (°/s).
// 32768.0f: Maximum value by a 16-bit register.
#define GYRO_SCALE_FACTOR (250.0f / 32768.0f)

// Configuration for the I2C master interface
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0

static uint8_t sensorAddr;

esp_err_t imu_init(uint8_t devAddr, gpio_num_t sda_pin, gpio_num_t scl_pin)
{
	sensorAddr = devAddr;

	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = sda_pin,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_io_num = scl_pin,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = I2C_MASTER_FREQ_HZ,
	};

	esp_err_t result_param_config = i2c_param_config(I2C_MASTER_NUM, &conf);
	switch (result_param_config)
	{
	case ESP_OK:
		break;
	default:
		printf("Failed to configure parameters: %s\n", esp_err_to_name(result_param_config));
		return ESP_ERR_NOT_FOUND;
	}

	esp_err_t result_driver_install = i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
	switch (result_driver_install)
	{
	case ESP_OK:
		break;
	default:
		printf("Failed to install driver: %s\n", esp_err_to_name(result_driver_install));
		return ESP_ERR_NOT_FOUND;
	}

	uint8_t buffer[14];
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, devAddr << 1 | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, 0x75, true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, devAddr << 1 | I2C_MASTER_READ, true);

	i2c_master_read(cmd, buffer, 14, I2C_MASTER_LAST_NACK);
	i2c_master_stop(cmd);

	esp_err_t result_cmd_begin_write = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
	switch (result_cmd_begin_write)
	{
	case ESP_OK:
		break;
	default:
		printf("Failed to send I2C commands: %s\n", esp_err_to_name(result_cmd_begin_write));
		i2c_cmd_link_delete(cmd);
		return ESP_ERR_NOT_FOUND;
	}

	i2c_cmd_link_delete(cmd);
	return ESP_OK;
}

esp_err_t imu_get_acceleration_data(AccelerationData *data)
{
	if (data == NULL)
	{
		return ESP_FAIL;
	}

	uint8_t buffer[6];
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (sensorAddr << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, ACCEL_REG_ADDR, true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (sensorAddr << 1) | I2C_MASTER_READ, true);
	i2c_master_read(cmd, buffer, 6, I2C_MASTER_LAST_NACK);
	i2c_master_stop(cmd);

	esp_err_t result_cmd_begin_write = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
	switch (result_cmd_begin_write)
	{
	case ESP_OK:
		break;
	default:
		printf("Failed to send I2C commands for acceleration data: %s\n", esp_err_to_name(result_cmd_begin_write));
		i2c_cmd_link_delete(cmd);
		return ESP_FAIL;
	}

	i2c_cmd_link_delete(cmd);
	data->accel_X = (float)((int16_t)((buffer[0] << 8) | buffer[1])) / ACCEL_SCALE_FACTOR;
	data->accel_Y = (float)((int16_t)((buffer[2] << 8) | buffer[3])) / ACCEL_SCALE_FACTOR;
	data->accel_Z = (float)((int16_t)((buffer[4] << 8) | buffer[5])) / ACCEL_SCALE_FACTOR;

	return ESP_OK;
}

esp_err_t imu_get_gyroscope_data(GyroscopeData *data)
{
	if (data == NULL)
	{
		return ESP_FAIL;
	}

	uint8_t buffer[6];
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (sensorAddr << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, GYRO_REG_ADDR, true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (sensorAddr << 1) | I2C_MASTER_READ, true);
	i2c_master_read(cmd, buffer, 6, I2C_MASTER_LAST_NACK);
	i2c_master_stop(cmd);

	esp_err_t result_cmd_begin_write = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
	switch (result_cmd_begin_write)
	{
	case ESP_OK:
		break;
	default:
		printf("Failed to send I2C commands for gyroscope data: %s\n", esp_err_to_name(result_cmd_begin_write));
		i2c_cmd_link_delete(cmd);
		return ESP_FAIL;
	}

	i2c_cmd_link_delete(cmd);

	data->rotation_X = (int16_t)roundf((float)((int16_t)((buffer[0] << 8) | buffer[1])) * GYRO_SCALE_FACTOR);
	data->rotation_Y = (int16_t)roundf((float)((int16_t)((buffer[2] << 8) | buffer[3])) * GYRO_SCALE_FACTOR);
	data->rotation_Z = (int16_t)roundf((float)((int16_t)((buffer[4] << 8) | buffer[5])) * GYRO_SCALE_FACTOR);

	return ESP_OK;
}

esp_err_t imu_deinit()
{
	esp_err_t result_driver_delete = i2c_driver_delete(I2C_MASTER_NUM);
	switch (result_driver_delete)
	{
	case ESP_OK:
		return ESP_OK;
	default:
		printf("Failed to delete driver: %s\n", esp_err_to_name(result_driver_delete));
		return ESP_FAIL;
	}
}