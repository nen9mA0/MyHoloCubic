#ifndef _MPU_H
#define _MPU_H

#include "freertos/FreeRTOS.h"
#include "driver/uart.h"

// Serial Mode
#define PIN_MPU_TXD 10
#define PIN_MPU_RXD 9

#define MPU_UART_PORT_NUM      2
#define MPU_UART_BAUD_RATE     115200
#define MPU_TASK_STACK_SIZE    1024

#define BUF_SIZE (1024)

static uint8_t checksum(uint8_t *data, size_t len);

esp_err_t mpu_serial_init(void);
void mpu_set_output(uint8_t output_bit);
uint8_t* mpu_get_data(int *len);

#endif
