/*
 * mpu6050_defs.h
 *
 * Definitions for MPU6050 IMU module.
 * Reference: InvenSense MPU6050 Register Map and Datasheet
 * Created on: Sep 9, 2025
 * Author: erol-tesla
 */

#ifndef MPU6050_INC_MPU6050_DEFS_H_
#define MPU6050_INC_MPU6050_DEFS_H_

#include <stdint.h>

/* -------------------------------------------------------------------------- */
/* Device ID                                                                  */
/* -------------------------------------------------------------------------- */
#define MPU6050_WHO_AM_I_ID        (0x68U)


/* -------------------------------------------------------------------------- */
/* Register map                                                               */
/* -------------------------------------------------------------------------- */
#define MPU6050_REG_SMPLRT_DIV     (0x19U)
#define MPU6050_REG_CONFIG         (0x1AU)
#define MPU6050_REG_GYRO_CONFIG    (0x1BU)
#define MPU6050_REG_ACCEL_CONFIG   (0x1CU)
#define MPU6050_REG_ACCEL_XOUT_H   (0x3BU)
#define MPU6050_REG_TEMP_OUT_H     (0x41U)
#define MPU6050_REG_GYRO_XOUT_H    (0x43U)
#define MPU6050_REG_PWR_MGMT_1     (0x6BU)
#define MPU6050_REG_PWR_MGMT_2     (0x6CU)
#define MPU6050_REG_WHO_AM_I       (0x75U)


/* -------------------------------------------------------------------------- */
/* Bit positions and masks                                                    */
/* -------------------------------------------------------------------------- */
/** @defgroup MPU6050_Bits Bit masks
 *  @brief Control and config bit positions/masks
 *  @{
 */
#define MPU6050_PWR1_SLEEP_Pos     (6U)
#define MPU6050_PWR1_SLEEP_Msk     (0x1U << MPU6050_PWR1_SLEEP_Pos)

#define MPU6050_PWR1_TEMP_DIS_Pos  (3U)
#define MPU6050_PWR1_TEMP_DIS_Msk  (0x1U << MPU6050_PWR1_TEMP_DIS_Pos)

#define MPU6050_PWR1_CLKSEL_Pos    (0U)
#define MPU6050_PWR1_CLKSEL_Msk    (0x7U << MPU6050_PWR1_CLKSEL_Pos)

#define MPU6050_ACCEL_FS_SEL_Pos   (3U)
#define MPU6050_ACCEL_FS_SEL_Msk   (0x3U << MPU6050_ACCEL_FS_SEL_Pos)

#define MPU6050_GYRO_FS_SEL_Pos    (3U)
#define MPU6050_GYRO_FS_SEL_Msk    (0x3U << MPU6050_GYRO_FS_SEL_Pos)
/** @} */


/* -------------------------------------------------------------------------- */
/* Accelerometer range                                                        */
/* -------------------------------------------------------------------------- */
/** @defgroup MPU6050_AccelRange Accelerometer range
 *  @brief Full-scale selection for accelerometer
 *  @{
 */
#define MPU6050_ACCEL_RANGE_2G     (0x0U << MPU6050_ACCEL_FS_SEL_Pos)
#define MPU6050_ACCEL_RANGE_4G     (0x1U << MPU6050_ACCEL_FS_SEL_Pos)
#define MPU6050_ACCEL_RANGE_8G     (0x2U << MPU6050_ACCEL_FS_SEL_Pos)
#define MPU6050_ACCEL_RANGE_16G    (0x3U << MPU6050_ACCEL_FS_SEL_Pos)
/** @} */


/* -------------------------------------------------------------------------- */
/* Gyroscope range                                                            */
/* -------------------------------------------------------------------------- */
/** @defgroup MPU6050_GyroRange Gyroscope range
 *  @brief Full-scale selection for gyroscope
 *  @{
 */
#define MPU6050_GYRO_RANGE_250DPS   (0x0U << MPU6050_GYRO_FS_SEL_Pos)
#define MPU6050_GYRO_RANGE_500DPS   (0x1U << MPU6050_GYRO_FS_SEL_Pos)
#define MPU6050_GYRO_RANGE_1000DPS  (0x2U << MPU6050_GYRO_FS_SEL_Pos)
#define MPU6050_GYRO_RANGE_2000DPS  (0x3U << MPU6050_GYRO_FS_SEL_Pos)
/** @} */


/* -------------------------------------------------------------------------- */
/* Digital low-pass filter                                                    */
/* -------------------------------------------------------------------------- */
/** @defgroup MPU6050_DLPF Digital low-pass filter
 *  @brief Configurations for DLPF (0–6)
 *  @{
 */
#define MPU6050_DLPF_CFG_0         (0x00U)
#define MPU6050_DLPF_CFG_1         (0x01U)
#define MPU6050_DLPF_CFG_2         (0x02U)
#define MPU6050_DLPF_CFG_3         (0x03U)
#define MPU6050_DLPF_CFG_4         (0x04U)
#define MPU6050_DLPF_CFG_5         (0x05U)
#define MPU6050_DLPF_CFG_6         (0x06U)
/** @} */


/* -------------------------------------------------------------------------- */
/* Sensitivity scale factors                                                  */
/* -------------------------------------------------------------------------- */
/** @defgroup MPU6050_Scale Sensitivity scale factors
 *  @brief LSB per physical unit
 *  @{
 */
#define MPU6050_ACCEL_SENS_2G      (16384.0f)
#define MPU6050_ACCEL_SENS_4G      (8192.0f)
#define MPU6050_ACCEL_SENS_8G      (4096.0f)
#define MPU6050_ACCEL_SENS_16G     (2048.0f)

#define MPU6050_GYRO_SENS_250DPS   (131.0f)
#define MPU6050_GYRO_SENS_500DPS   (65.5f)
#define MPU6050_GYRO_SENS_1000DPS  (32.8f)
#define MPU6050_GYRO_SENS_2000DPS  (16.4f)
/** @} */

#endif /* MPU6050_INC_MPU6050_DEFS_H_ */
