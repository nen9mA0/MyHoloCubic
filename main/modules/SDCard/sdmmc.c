#include "sdmmc.h"

static const char *TAG = "sdcard";

const char mount_point[] = MOUNT_POINT;

sdmmc_host_t host = SDMMC_HOST_DEFAULT();
sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = false,
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
};
sdmmc_card_t* card;


esp_err_t sdmmc_init(void)
{
    esp_err_t ret;

    ESP_LOGI(TAG, "Initializing SD card");
    ESP_LOGI(TAG, "Using SDMMC peripheral");

    host.max_freq_khz = SDMMC_FREQ_DEFAULT;
    // 1 bit mode
    slot_config.width = 1;

    gpio_set_pull_mode(15, GPIO_PULLUP_ONLY);   // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);    // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode(13, GPIO_PULLUP_ONLY);   // D3, needed in 4- and 1-line modes

    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return ret;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    return ret;
}

esp_err_t sdmmc_deinit(void)
{
    // All done, unmount partition and disable SDMMC or SPI peripheral
    esp_err_t err;
    err = esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(TAG, "Card unmounted");
    err = spi_bus_free(host.slot);
    return err;
}
