#include "mpu.h"

static uint8_t data[BUF_SIZE];

esp_err_t mpu_serial_init(void)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = MPU_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(MPU_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(MPU_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(MPU_UART_PORT_NUM, PIN_MPU_TXD, PIN_MPU_RXD, -1, -1));

    return 0;
}

static uint8_t checksum(uint8_t *data, size_t len)
{
    uint8_t sum = 0;
    for(size_t i=0; i<len; i++)
        sum += data[i];
    return sum;
}

void mpu_set_output(uint8_t output_bit)
{
    uint8_t send_data[4];

    send_data[0]= 0xa5;
    send_data[1]= 0x55;
    send_data[2]= output_bit;
    send_data[3]= checksum(send_data, 3);
    uart_write_bytes(MPU_UART_PORT_NUM, (const char *) send_data, 4);
}

uint8_t* mpu_get_data(int *len)
{
    uint8_t send_data[4];

    send_data[0]= 0xa5;
    send_data[1]= 0x56;
    send_data[2]= 0x01;
    send_data[3]= 0xfc;
    uart_write_bytes(MPU_UART_PORT_NUM, (const char *) send_data, 4);
    int ret_len = uart_read_bytes(MPU_UART_PORT_NUM, data, BUF_SIZE, 20 / portTICK_RATE_MS);

    *len = ret_len;

    return data;
}

    // while (1) {
    //     // Read data from the UART
    //     int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, BUF_SIZE, 20 / portTICK_RATE_MS);
    //     // Write data back to the UART
    //     uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) data, len);
    // }
