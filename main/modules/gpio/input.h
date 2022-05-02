#ifndef _INPUT_H
#define _INPUT_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define GPIO_INPUT_IO_0     37
#define GPIO_INPUT_IO_1     36
#define GPIO_INPUT_IO_2     34
#define GPIO_INPUT_IO_3     35
#define GPIO_INPUT_PIN_SEL  ( (1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1) | (1ULL<<GPIO_INPUT_IO_2)  | (1ULL<<GPIO_INPUT_IO_3) )
#define ESP_INTR_FLAG_DEFAULT 0

void input_init();
void gpio_task(void* arg);

#endif