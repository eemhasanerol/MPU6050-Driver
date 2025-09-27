/**
  ******************************************************************************
  * @file    mpu6050.c
  * @author  Hasan Erol
  * @brief   MPU6050 driver source file
  ******************************************************************************
  * @attention
  * For usage examples, see README.md
  ******************************************************************************
  */

#include "mpu6050.h"

mpu6050_status_t mpu6050_init(mpu6050_dev_t *dev)
{
    uint8_t v, reg;
    uint32_t to;

    /* WHO_AM_I check */
    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_WHO_AM_I, &v, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    if (v != MPU6050_WHO_AM_I_ID) {
        return MPU6050_E_ID;
    }

    /* Device reset */
    v = 0x80U;
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_PWR_MGMT_1, &v, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    to = 200;
    do {
        if (dev->i2c_read(dev->dev_addr, MPU6050_REG_PWR_MGMT_1, &v, 1U) != MPU6050_OK) {
            return MPU6050_E_COMM;
        }
        if ((v & 0x80U) == 0U) {
            break;
        }
        for (volatile int d = 0; d < 160000; d++);
    } while (--to);
    if (to == 0U) {
        return MPU6050_E_TIMEOUT;
    }

    /* Wake up and select clock */
    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_PWR_MGMT_1, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    reg &= ~(MPU6050_PWR1_SLEEP_Msk | 0x07U);
    reg |= 0x01U;
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_PWR_MGMT_1, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    /* Enable all axes */
    v = 0x00U;
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_PWR_MGMT_2, &v, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    /* Signal path reset */
    v = 0x07U;
    if (dev->i2c_write(dev->dev_addr, 0x68U, &v, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    /* Configure DLPF */
    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_CONFIG, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    reg &= ~0x07U;
    reg |= (dev->dlpf_cfg & 0x07U);
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_CONFIG, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    /* Sample rate divider */
    v = (uint8_t)dev->sample_rate;
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_SMPLRT_DIV, &v, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    /* Gyroscope range */
    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_GYRO_CONFIG, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    reg &= ~MPU6050_GYRO_FS_SEL_Msk;
    reg |= (dev->gyro_range & MPU6050_GYRO_FS_SEL_Msk);
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_GYRO_CONFIG, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    /* Accelerometer range */
    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_ACCEL_CONFIG, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    reg &= ~MPU6050_ACCEL_FS_SEL_Msk;
    reg |= (dev->accel_range & MPU6050_ACCEL_FS_SEL_Msk);
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_ACCEL_CONFIG, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    return MPU6050_OK;
}

mpu6050_status_t mpu6050_read_all(mpu6050_dev_t *dev, mpu6050_data_t *data)
{
    if (mpu6050_read_accel(dev, data) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    
    if (mpu6050_read_gyro(dev, data) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    
    if (mpu6050_read_temp(dev, data) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_read_accel(mpu6050_dev_t *dev, mpu6050_data_t *data)
{
    uint8_t buf[6];
    float scale = 1.0f;

    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_ACCEL_XOUT_H, buf, 6U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    int16_t accel_raw_x = (int16_t)((buf[0] << 8) | buf[1]);
    int16_t accel_raw_y = (int16_t)((buf[2] << 8) | buf[3]);
    int16_t accel_raw_z = (int16_t)((buf[4] << 8) | buf[5]);

    data->accel_raw[0] = accel_raw_x;
    data->accel_raw[1] = accel_raw_y;
    data->accel_raw[2] = accel_raw_z;

    switch (dev->accel_range) {
        case MPU6050_ACCEL_RANGE_2G:  scale = 1.0f / MPU6050_ACCEL_SENS_2G;  break;
        case MPU6050_ACCEL_RANGE_4G:  scale = 1.0f / MPU6050_ACCEL_SENS_4G;  break;
        case MPU6050_ACCEL_RANGE_8G:  scale = 1.0f / MPU6050_ACCEL_SENS_8G;  break;
        case MPU6050_ACCEL_RANGE_16G: scale = 1.0f / MPU6050_ACCEL_SENS_16G; break;
        default: break;
    }

    data->accel_g[0] = (float)accel_raw_x * scale;
    data->accel_g[1] = (float)accel_raw_y * scale;
    data->accel_g[2] = (float)accel_raw_z * scale;

    return MPU6050_OK;
}

mpu6050_status_t mpu6050_read_gyro(mpu6050_dev_t *dev, mpu6050_data_t *data)
{
    uint8_t buf[6];
    float scale = 1.0f;

    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_GYRO_XOUT_H, buf, 6U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    int16_t gyro_raw_x = (int16_t)((buf[0] << 8) | buf[1]);
    int16_t gyro_raw_y = (int16_t)((buf[2] << 8) | buf[3]);
    int16_t gyro_raw_z = (int16_t)((buf[4] << 8) | buf[5]);

    data->gyro_raw[0] = gyro_raw_x;
    data->gyro_raw[1] = gyro_raw_y;
    data->gyro_raw[2] = gyro_raw_z;

    switch (dev->gyro_range) {
        case MPU6050_GYRO_RANGE_250DPS : scale = 1.0f / MPU6050_GYRO_SENS_250DPS;  break;
        case MPU6050_GYRO_RANGE_500DPS : scale = 1.0f / MPU6050_GYRO_SENS_500DPS;  break;
        case MPU6050_GYRO_RANGE_1000DPS: scale = 1.0f / MPU6050_GYRO_SENS_1000DPS; break;
        case MPU6050_GYRO_RANGE_2000DPS: scale = 1.0f / MPU6050_GYRO_SENS_2000DPS; break;
        default: break;
    }

    data->gyro_dps[0] = (float)gyro_raw_x * scale;
    data->gyro_dps[1] = (float)gyro_raw_y * scale;
    data->gyro_dps[2] = (float)gyro_raw_z * scale;

    return MPU6050_OK;
}

mpu6050_status_t mpu6050_read_temp(mpu6050_dev_t *dev, mpu6050_data_t *data)
{
    uint8_t buf[2];

    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_PWR_MGMT_1, buf, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    if ((buf[0] & MPU6050_PWR1_TEMP_DIS_Msk) == 0U) {
        if (dev->i2c_read(dev->dev_addr, MPU6050_REG_TEMP_OUT_H, buf, 2U) != MPU6050_OK) {
            return MPU6050_E_COMM;
        }

        int16_t temp_raw = (int16_t)((buf[0] << 8U) | buf[1]);
        data->temp_raw = temp_raw;
        data->temp_c   = (float)temp_raw / 340.0f + 36.53f;
    } else {
        data->temp_raw = 0;
        data->temp_c   = 0.0f;
    }

    return MPU6050_OK;
}

mpu6050_status_t mpu6050_read_whoami(mpu6050_dev_t *dev, uint8_t *whoami)
{
    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_WHO_AM_I, whoami, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_set_accel_range(mpu6050_dev_t *dev, uint8_t accel_range)
{
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_ACCEL_CONFIG, &accel_range, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_set_gyro_range(mpu6050_dev_t *dev, uint8_t gyro_range)
{
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_GYRO_CONFIG, &gyro_range, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_set_dlpf(mpu6050_dev_t *dev, uint8_t dlpf_cfg)
{
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_CONFIG, &dlpf_cfg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_set_sample_rate(mpu6050_dev_t *dev, uint8_t smplrt_div)
{
    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_SMPLRT_DIV, &smplrt_div, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }
    
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_set_sleep(mpu6050_dev_t *dev, uint8_t enable)
{
    uint8_t reg;

    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_PWR_MGMT_1, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    if (enable) {
        reg |= MPU6050_PWR1_SLEEP_Msk;
    } else {
        reg &= ~MPU6050_PWR1_SLEEP_Msk;
    }

    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_PWR_MGMT_1, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    return MPU6050_OK;
}

mpu6050_status_t mpu6050_set_temp_enable(mpu6050_dev_t *dev, uint8_t enable)
{
    uint8_t reg;

    if (dev->i2c_read(dev->dev_addr, MPU6050_REG_PWR_MGMT_1, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    if (enable) {
        reg &= ~MPU6050_PWR1_TEMP_DIS_Msk;
    } else {
        reg |= MPU6050_PWR1_TEMP_DIS_Msk;
    }

    if (dev->i2c_write(dev->dev_addr, MPU6050_REG_PWR_MGMT_1, &reg, 1U) != MPU6050_OK) {
        return MPU6050_E_COMM;
    }

    return MPU6050_OK;
}


