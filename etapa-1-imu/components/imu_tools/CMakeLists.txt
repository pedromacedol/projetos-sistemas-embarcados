idf_component_register(
  SRCS "imu_tools.c"
  INCLUDE_DIRS "include"
  PRIV_REQUIRES sensor_imu
  REQUIRES driver esp_common
)