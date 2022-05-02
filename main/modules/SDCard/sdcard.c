#include "sdcard.h"

const char mount_point[] = MOUNT_POINT;

sdmmc_host_t host = SDSPI_HOST_DEFAULT();
sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = false,
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
};
sdmmc_card_t* card;


esp_err_t sdcard_init(void)
{
    esp_err_t ret;

    ESP_LOGI(SD_TAG, "Initializing SD card");
    ESP_LOGI(SD_TAG, "Using SPI peripheral");

    // host : SDSPI_HOST_DEFAULT()
    host.max_freq_khz = SDMMC_FREQ_500K;
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ret = spi_bus_initialize(host.slot, &bus_cfg, SD_SPI_DMA_CHAN);
    if (ret != ESP_OK) {
        ESP_LOGE(SD_TAG, "Failed to initialize bus.");
        return ret;
    }

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    // slot_config : SDSPI_DEVICE_CONFIG_DEFAULT()
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;

    //esp_log_level_set("sdmmc_init", ESP_LOG_INFO);        for debug
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);


    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(SD_TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(SD_TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return ret;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    return ret;
}

esp_err_t sdcard_deinit(void)
{
    // All done, unmount partition and disable SDMMC or SPI peripheral
    esp_err_t err;
    err = esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(SD_TAG, "Card unmounted");
    err = spi_bus_free(host.slot);
    return err;
}
