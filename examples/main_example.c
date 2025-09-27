/*
 * main_example.c
 *
 * Example usage of MPU6050 driver
 *
 * Tested on: STM32F407 (custom drivers for RCC, GPIO, I2C)
 */

#include "stm32f407xx.h"
#include "RCC.h"
#include "GPIO.h"
#include "I2C.h"
#include "mpu6050.h"
#include <stdio.h>


/* -------- I2C handle -------- */
static I2C_HandleTypeDef_t hi2c1;

/* -------- Simple delay (blocking) -------- */
static void platform_delay_ms(uint32_t ms)
{
    for (volatile uint32_t i = 0; i < (ms * 8000U); i++);
}

/* -------- I2C wrapper functions -------- */
static int32_t platform_i2c_read(uint8_t dev, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (I2C_MemRead(&hi2c1, dev, reg, buf, len) != STATUS_OK) {
        return MPU6050_E_COMM;
    }
    return MPU6050_OK;
}

static int32_t platform_i2c_write(uint8_t dev, uint8_t reg, const uint8_t *buf, uint16_t len)
{
    if (I2C_MemWrite(&hi2c1, dev, reg, buf, len) != STATUS_OK) {
        return MPU6050_E_COMM;
    }
    return MPU6050_OK;
}

/* -------- Init functions -------- */
static void I2C1_InitPins(void)
{
    RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef_t I2C_Pins = {0};
    I2C_Pins.pinNumber  = (GPIO_PIN_6 | GPIO_PIN_7);  // PB6 = SCL, PB7 = SDA
    I2C_Pins.Mode       = GPIO_MODE_AF;
    I2C_Pins.Otype      = GPIO_OTYPE_OD;
    I2C_Pins.PuPd       = GPIO_PULLUP;
    I2C_Pins.Speed      = GPIO_SPEED_HIGH;
    I2C_Pins.Alternate  = GPIO_AF4_I2C1;  // AF4 = I2C1

    GPIO_Init(GPIOB, &I2C_Pins);
}

static void I2C1_InitPeripheral(void)
{
    RCC_I2C1_CLK_ENABLE();

    hi2c1.inst                 = I2C1;
    hi2c1.config.ack           = I2C_ACK_ENABLE;
    hi2c1.dev_addr             = 0x00;                /* master own addr */
    hi2c1.config.fm_duty       = I2C_FM_DUTY_2;
    hi2c1.config.scl_speed_hz  = I2C_SPEED_STANDARD;  /* 100 kHz */
    hi2c1.timeout_ms           = 1000;

    I2C_Init(&hi2c1);
}

/* -------- Main -------- */
int main(void)
{
    /* Init I2C1 */
    I2C1_InitPins();
    I2C1_InitPeripheral();

    /* MPU6050 handle */
    mpu6050_dev_t mpu = {
        .dev_addr    = MPU6050_I2C_ADDR_AD0_LOW,   /* 0x68 (AD0 = GND) */
        .accel_range = MPU6050_ACCEL_RANGE_2G,
        .gyro_range  = MPU6050_GYRO_RANGE_250DPS,
        .dlpf_cfg    = MPU6050_DLPF_CFG_4,         /* ~44 Hz LPF */
        .sample_rate = MPU6050_SMPLRT_20HZ,        /* 20 Hz */
        .i2c_read    = platform_i2c_read,
        .i2c_write   = platform_i2c_write
    };

    /* Init sensor */
    if (mpu6050_init(&mpu) != MPU6050_OK) {
        while (1); /* error */
    }

    mpu6050_data_t mpu_data;

    while (1)
    {
        if (mpu6050_read_all(&mpu, &mpu_data) == MPU6050_OK)
        {
            printf("AX: %.2f g  AY: %.2f g  AZ: %.2f g | "
                   "GX: %.2f dps  GY: %.2f dps  GZ: %.2f dps | "
                   "Temp: %.2f °C\r\n",
                   mpu_data.accel_g[0], mpu_data.accel_g[1], mpu_data.accel_g[2],
                   mpu_data.gyro_dps[0], mpu_data.gyro_dps[1], mpu_data.gyro_dps[2],
                   mpu_data.temp_c);
        }

        platform_delay_ms(1000); /* 1s delay */
    }
}
