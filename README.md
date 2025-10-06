# MPU6050 Driver (C)

A simple C driver library for the **MPU6050 IMU sensor** (accelerometer + gyroscope + temperature).  
Tested on STM32F407 with custom low-level drivers.

---

## Features
- I²C communication (with user-provided low-level functions)
- Read accelerometer data (X, Y, Z)
- Read gyroscope data (X, Y, Z)
- Read temperature (°C)
- Configuration options:
  - Accelerometer full-scale range: ±2g / ±4g / ±8g / ±16g
  - Gyroscope full-scale range: ±250 / ±500 / ±1000 / ±2000 dps
  - Digital Low-Pass Filter (DLPF)
  - Output sample rate
  - Sleep mode and temperature sensor enable/disable

---

## Usage Example
```c
/* Platform-specific I²C read/write functions must be provided by the user */
#include "mpu6050.h"

mpu6050_dev_t mpu = {
    .dev_addr    = 0x68,
    .accel_range = MPU6050_ACCEL_RANGE_2G,
    .gyro_range  = MPU6050_GYRO_RANGE_250DPS,
    .dlpf_cfg    = MPU6050_DLPF_CFG_3,
    .sample_rate = 50,
    .i2c_read    = platform_i2c_read,
    .i2c_write   = platform_i2c_write
};

mpu6050_data_t data;

if (mpu6050_init(&mpu) == MPU6050_OK) {
    while (1) {
        if (mpu6050_read_all(&mpu, &data) == MPU6050_OK) {
            printf("AX=%.2f AY=%.2f AZ=%.2f | GX=%.2f GY=%.2f GZ=%.2f | Temp=%.2f\n",
                   data.accel_g[0], data.accel_g[1], data.accel_g[2],
                   data.gyro_dps[0], data.gyro_dps[1], data.gyro_dps[2],
                   data.temp_c);
        }
    }
}
