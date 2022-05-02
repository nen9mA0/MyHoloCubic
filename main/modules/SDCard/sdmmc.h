#ifndef _SDMMC_H
#define _SDMMC_H


// for sd card
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "sdkconfig.h"


// ==== SD Card ====

#define MOUNT_POINT "/sdcard"

#define PIN_NUM_DAT0 2
#define PIN_NUM_CMD  15
// #define PIN_NUM_CLK  14
#define PIN_NUM_CD   13


// 没上电：108
// 没插卡：107


esp_err_t sdmmc_init(void);
esp_err_t sdmmc_deinit(void);

#endif