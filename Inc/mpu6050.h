/*
 * mpu6050.h
 *
 *  Created on: Sep 9, 2025
 *      Author: erol-tesla
 */

#ifndef MPU6050_INC_MPU6050_H_
#define MPU6050_INC_MPU6050_H_

#include <stdint.h>
#include "mpu6050_defs.h"


/**
 * @brief Status codes for MPU6050 driver functions.
 *
 * These codes are returned by all public API functions
 * to indicate success or type of error.
 */
typedef enum {
    MPU6050_OK      = 0,   /*!< Operation successful */
    MPU6050_E_COMM  = -1,  /*!< Communication error (I2C/SPI) */
    MPU6050_E_ID    = -2,  /*!< Wrong chip ID */
    MPU6050_E_PARAM = -3,   /*!< Invalid parameter */
	MPU6050_E_TIMEOUT = -4
} mpu6050_status_t;


/* -------------------------------------------------------------------------- */
/* Device handle                                                              */
/* -------------------------------------------------------------------------- */
/**
 * @brief MPU6050 device configuration and I2C hooks
 */
typedef struct {
    uint8_t  dev_addr;     /*!< I2C device address: 0x68 or 0x69 */

    uint8_t  accel_range;  /*!< Accelerometer full-scale range.
                                @ref MPU6050_ACCEL_RANGE_* */

    uint8_t  gyro_range;   /*!< Gyroscope full-scale range.
                                @ref MPU6050_GYRO_RANGE_* */

    uint8_t  dlpf_cfg;     /*!< Digital Low Pass Filter setting (0–6).
                                @ref MPU6050_DLPF_CFG_* */

    uint16_t sample_rate;  /*!< Output sample rate in Hz.
    						@defgroup MPU6050_SampleRate_* */


    /* I2C callbacks */
    int32_t (*i2c_read)(uint8_t dev, uint8_t reg, uint8_t *buf, uint16_t len);
    int32_t (*i2c_write)(uint8_t dev, uint8_t reg, const uint8_t *buf, uint16_t len);
} mpu6050_dev_t;


/* -------------------------------------------------------------------------- */
/* Data structure (raw + scaled values)                                       */
/* -------------------------------------------------------------------------- */
/**
 * @brief MPU6050 raw sensor outputs and scaled data
 */
typedef struct {
    int16_t accel_raw[3];  /*!< Raw accelerometer data: X, Y, Z */
    int16_t gyro_raw[3];   /*!< Raw gyroscope data: X, Y, Z */
    int16_t temp_raw;      /*!< Raw temperature data */

    float accel_g[3];      /*!< Scaled accelerometer data in g */
    float gyro_dps[3];     /*!< Scaled gyroscope data in dps */
    float temp_c;          /*!< Temperature in °C */
} mpu6050_data_t;


/* -------------------------------------------------------------------------- */
/* API function prototypes                                                    */
/* -------------------------------------------------------------------------- */
/**
 * @brief Initialize the MPU6050 with provided configuration.
 * @param  dev Device handle
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_init(mpu6050_dev_t *dev);

/**
 * @brief Read accelerometer, gyroscope, and temperature data.
 * @param  dev  Device handle
 * @param  data Output data structure
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_read_all(mpu6050_dev_t *dev, mpu6050_data_t *data);

/**
 * @brief Read only accelerometer data.
 * @param  dev  Device handle
 * @param  data Output structure (only accel updated)
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_read_accel(mpu6050_dev_t *dev, mpu6050_data_t *data);

/**
 * @brief Read only gyroscope data.
 * @param  dev  Device handle
 * @param  data Output structure (only gyro updated)
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_read_gyro(mpu6050_dev_t *dev, mpu6050_data_t *data);

/**
 * @brief Read only temperature data.
 * @param  dev  Device handle
 * @param  data Output structure (only temp updated)
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_read_temp(mpu6050_dev_t *dev, mpu6050_data_t *data);

/**
 * @brief Read WHO_AM_I register (should return 0x68).
 * @param  dev     Device handle
 * @param  whoami  Returned WHO_AM_I value
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_read_whoami(mpu6050_dev_t *dev, uint8_t *whoami);

/**
 * @brief Set accelerometer full-scale range.
 * @param  dev         Device handle
 * @param  accel_range Range setting @ref MPU6050_ACCEL_RANGE_*
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_set_accel_range(mpu6050_dev_t *dev, uint8_t accel_range);

/**
 * @brief Set gyroscope full-scale range.
 * @param  dev        Device handle
 * @param  gyro_range Range setting @ref MPU6050_GYRO_RANGE_*
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_set_gyro_range(mpu6050_dev_t *dev, uint8_t gyro_range);

/**
 * @brief Set Digital Low Pass Filter (DLPF) config.
 * @param  dev      Device handle
 * @param  dlpf_cfg Filter config @ref MPU6050_DLPF_CFG_*
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_set_dlpf(mpu6050_dev_t *dev, uint8_t dlpf_cfg);

/**
 * @brief Set sample rate divider.
 * @param  dev        Device handle
 * @param  smplrt_div Divider value
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_set_sample_rate(mpu6050_dev_t *dev, uint8_t smplrt_div);

/**
 * @brief Enable or disable sleep mode.
 * @param  dev    Device handle
 * @param  enable 1 = sleep, 0 = wake
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_set_sleep(mpu6050_dev_t *dev, uint8_t enable);

/**
 * @brief Enable or disable temperature sensor.
 * @param  dev    Device handle
 * @param  enable 1 = enable, 0 = disable
 * @return mpu6050_status_t
 */
mpu6050_status_t mpu6050_set_temp_enable(mpu6050_dev_t *dev, uint8_t enable);

#endif /* MPU6050_INC_MPU6050_H_ */
