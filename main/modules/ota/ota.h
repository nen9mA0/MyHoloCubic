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

// #define CRC32_CHECK
// #define HASH_TYPE "CRC32"
// #define HASH_LEN 4
// #define CRC_TABLE_LEN 256
// #define CHECKSUM_PATH "/upg_dev_crc"

#define SHA256_CHECK
#define HASH_TYPE "SHA256"
#define HASH_LEN 32
#define CHECKSUM_PATH "/upg_dev_sha"

#if defined(CRC32_CHECK) || defined(SHA256_CHECK)
#define HASH_CHECK
#endif

#define OTA_RECV_TIMEOUT 5000

esp_err_t ota_start(char *url);

#ifdef HASH_CHECK
int Hex2Num(char *p, int len);
void print_checksum (const char *image_hash, const char *label);
#endif

#ifdef CRC32_CHECK
uint32_t* CRC32_init(uint32_t* table);
uint32_t CRC32_checksum(uint32_t crc, uint32_t *crc_table, char* buf, int len);
#endif

#endif