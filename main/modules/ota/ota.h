#ifndef _OTA_H
#define _OTA_H

#include <string.h>

#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"
#include "errno.h"
#include "esp_log.h"

#include "driver/gpio.h"

#include "../http/myhttp.h"

#define CRC32_CHECK
#define OTA_RECV_TIMEOUT 5000

esp_err_t ota_start(char *url);

uint32_t* CRC32_init(uint32_t* table);
uint32_t CRC32_checksum(uint32_t crc, uint32_t *crc_table, char* buf, int len);
uint32_t Hex2Num(char *p, int len);

#endif