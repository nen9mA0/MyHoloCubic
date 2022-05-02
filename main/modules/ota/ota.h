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

#define OTA_RECV_TIMEOUT 5000

esp_err_t ota_start(char *url);

#endif