#ifndef _SDCARD_H
#define _SDCARD_H


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

static const char *SD_TAG = "sdcard";

#define MOUNT_POINT "/sdcard"

#define USE_SPI_MODE

// DMA channel to be used by the SPI peripheral
#ifndef SD_SPI_DMA_CHAN
#define SD_SPI_DMA_CHAN    SPI_DMA_CH_AUTO
#endif //SPI_DMA_CHAN

#define PIN_NUM_MISO 2
#define PIN_NUM_MOSI 13
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   15


// 没上电：108
// 没插卡：107


esp_err_t sdcard_init(void);
esp_err_t sdcard_deinit(void);

#endif