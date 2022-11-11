#include "ota.h"

#define BUFFSIZE 1024

static const char TAG[] = "OTA";
static char ota_write_data[BUFFSIZE + 1] = {0};

/*
    return value:
        -1 for error
        0  for success update
        1  for current version is the highest version
*/
esp_err_t ota_start(char *url)
{
    esp_err_t err;
    /* update handle : set by esp_ota_begin(), must be freed via esp_ota_end() */
    esp_ota_handle_t update_handle = -1;
    const esp_partition_t *update_partition = NULL;
    int data_read;

#ifdef HASH_CHECK
    uint8_t checksum[HASH_LEN] = {0};
#endif
#ifdef CRC32_CHECK
    uint32_t crc = 0;
    static uint32_t crc_table[CRC_TABLE_LEN];
#endif

    ESP_LOGI(TAG, "Starting OTA example");

    const esp_partition_t *configured = esp_ota_get_boot_partition();
    const esp_partition_t *running = esp_ota_get_running_partition();

    if (configured != running)
    {
        ESP_LOGW(TAG, "Configured OTA boot partition at offset 0x%08x, but running from offset 0x%08x",
                 configured->address, running->address);
        ESP_LOGW(TAG, "(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
    }
    ESP_LOGI(TAG, "Running partition type %d subtype %d (offset 0x%08x)",
             running->type, running->subtype, running->address);

    esp_http_client_handle_t client = ClientInit(url, "/upg_dev");
    if (client)
    {
        err = ClientOpen(client);
        if (err >= 0)
        {
            update_partition = esp_ota_get_next_update_partition(NULL);
            ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%x",
                     update_partition->subtype, update_partition->address);
            assert(update_partition != NULL);

            int binary_file_length = 0;
            /*deal with all receive packet*/
            bool image_header_was_checked = false;
#ifdef CRC32_CHECK
            CRC32_init(crc_table);
#endif
            while (1)
            {
                data_read = ClientRead(client, ota_write_data, BUFFSIZE);
                if (data_read > 0)
                {
                    if (image_header_was_checked == false)
                    {
                        esp_app_desc_t new_app_info;
                        if (data_read > sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t))
                        {
                            // check current version with downloading
                            memcpy(&new_app_info, &ota_write_data[sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t)], sizeof(esp_app_desc_t));
                            ESP_LOGI(TAG, "New firmware version: %s", new_app_info.version);

                            esp_app_desc_t running_app_info;
                            if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK)
                            {
                                ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
                            }

                            const esp_partition_t *last_invalid_app = esp_ota_get_last_invalid_partition();
                            esp_app_desc_t invalid_app_info;
                            if (esp_ota_get_partition_description(last_invalid_app, &invalid_app_info) == ESP_OK)
                            {
                                ESP_LOGI(TAG, "Last invalid firmware version: %s", invalid_app_info.version);
                            }

                            // check current version with last invalid partition
                            if (last_invalid_app != NULL)
                            {
                                if (memcmp(invalid_app_info.version, new_app_info.version, sizeof(new_app_info.version)) == 0)
                                {
                                    ESP_LOGW(TAG, "New version is the same as invalid version.");
                                    ESP_LOGW(TAG, "Previously, there was an attempt to launch the firmware with %s version, but it failed.", invalid_app_info.version);
                                    ESP_LOGW(TAG, "The firmware has been rolled back to the previous version.");
                                    ClientCleanup(client);
                                    return 1;
                                }
                            }
#ifndef CONFIG_EXAMPLE_SKIP_VERSION_CHECK
                            if (memcmp(new_app_info.version, running_app_info.version, sizeof(new_app_info.version)) == 0)
                            {
                                ESP_LOGW(TAG, "Current running version is the same as a new. We will not continue the update.");
                                ClientCleanup(client);
                                return 1;
                            }
#endif

                            image_header_was_checked = true;

                            err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
                            if (err != ESP_OK)
                            {
                                ESP_LOGE(TAG, "esp_ota_begin failed (%s)", esp_err_to_name(err));
                                ClientCleanup(client);
                                esp_ota_abort(update_handle);
                                return -1;
                            }
                            ESP_LOGI(TAG, "esp_ota_begin succeeded");
                        }
                        else
                        {
                            ESP_LOGE(TAG, "received package is not fit len");
                            ClientCleanup(client);
                            esp_ota_abort(update_handle);
                            return -1;
                        }
                    }
#ifdef CRC32_CHECK
                    // ESP_LOGI(TAG, "Len: %d  CRC midnum: %08X", data_read, crc);
                    crc = CRC32_checksum(crc, crc_table, ota_write_data, data_read);
#endif
                    err = esp_ota_write(update_handle, (const void *)ota_write_data, data_read);
                    if (err != ESP_OK)
                    {
                        ClientCleanup(client);
                        esp_ota_abort(update_handle);
                        return -1;
                    }
                    binary_file_length += data_read;
                    ESP_LOGD(TAG, "Written image length %d", binary_file_length);
                }
                else if (data_read == 0)
                {
                    break;
                }
                else
                {
                    ESP_LOGE(TAG, "ClientRead Error");
                    if (image_header_was_checked)
                        esp_ota_abort(update_handle);
                    return -1;
                }
            }

            // New Firmware Download Done
            ESP_LOGI(TAG, "Total Write binary data length: %d", binary_file_length);
            if (esp_http_client_is_complete_data_received(client) != true)
            {
                ESP_LOGE(TAG, "Error in receiving complete file");
                ClientCleanup(client);
                esp_ota_abort(update_handle);
                return -1;
            }

            // All data received, close http client
            ClientCleanup(client);

            err = esp_ota_end(update_handle);
            if (err != ESP_OK)
            {
                if (err == ESP_ERR_OTA_VALIDATE_FAILED)
                {
                    ESP_LOGE(TAG, "Image validation failed, image is corrupted");
                }
                ESP_LOGE(TAG, "esp_ota_end failed (%s)!", esp_err_to_name(err));
                return -1;
            }

#ifdef HASH_CHECK
            int hash_len;
            client = ClientInit(url, CHECKSUM_PATH);
            if (client)
            {
                err = ClientOpen(client);
                if(err>=0)
                {
                    data_read = ClientRead(client, ota_write_data, BUFFSIZE);
                    if (data_read >= 0)
                    {
                        hash_len = Hex2Num(ota_write_data, data_read);
                        ClientCleanup(client);
                    }
                    else
                    {
                        ESP_LOGE(TAG, "Error when reading checksum");
                        return -1;
                    }
                }
            }
#endif

#ifdef CRC32_CHECK
            print_checksum(ota_write_data, "Original Image CRC");
            // Attention: the compare here we use a cast, which bytes order is inverted.
            //            So the checksum which server returned has inverted its bytes order
            *((uint32_t *)checksum) = crc;
            print_checksum((char *)checksum, "Image CRC");
#elif defined(SHA256_CHECK)
            print_checksum(ota_write_data, "Original Image SHA256");
            esp_partition_get_sha256(update_partition, checksum);
            print_checksum((char *)checksum, "Image SHA256");
#endif
#ifdef HASH_CHECK
            if (memcmp(checksum, ota_write_data, HASH_LEN) != 0)
            {
                ESP_LOGE(TAG, "Hash Check Failed");
                return -1;
            }
            else
            {
                ESP_LOGI(TAG, "Hash Check Done");
            }
#endif
            err = esp_ota_set_boot_partition(update_partition);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err));
                return -1;
            }
            ESP_LOGI(TAG, "Prepare to restart system!");
            esp_restart();
        }
    }
    return 0;
}

#ifdef HASH_CHECK
int Hex2Num(char *p, int len)
{
    int i = 0, j = 0, index = 0;
    uint8_t num = 0;
    while (i < len)
    {
        if (p[i] >= '0' && p[i] <= '9')
        {
            num = num << 4;
            num |= p[i] - '0';
            j++;
        }
        else if (p[i] >= 'a' && p[i] <= 'f')
        {
            num = num << 4;
            num |= p[i] - 'a' + 10;
            j++;
        }
        else if (p[i] >= 'A' && p[i] <= 'F')
        {
            num = num << 4;
            num |= p[i] - 'A' + 10;
            j++;
        }

        if (j % 2 == 0)
        {
            p[index] = num;
            index++;
        }
        i++;
    }
    return index;
}

void print_checksum(const char *image_hash, const char *label)
{
    char hash_print[HASH_LEN * 2 + 1];
    hash_print[HASH_LEN * 2] = 0;
    for (int i = 0; i < HASH_LEN; ++i)
    {
        sprintf(&hash_print[i * 2], "%02x", image_hash[i]);
    }
    ESP_LOGI(TAG, "%s: %s", label, hash_print);
}

#endif
#ifdef CRC32_CHECK
uint32_t *CRC32_init(uint32_t *table)
{
    uint32_t c;
    for (uint32_t i = 0; i < 256; i++)
    {
        c = i;
        for (int j = 0; j < 8; j++)
        {
            if (c & 1)
                c = 0xedb88320 ^ (c >> 1);
            else
                c = c >> 1;
        }
        table[i] = c;
    }
    return table;
}

uint32_t CRC32_checksum(uint32_t crc, uint32_t *crc_table, char *buf, int len)
{
    uint32_t crc_tmp = ~crc;
    for (int i = 0; i < len; i++)
    {
        crc_tmp = crc_table[(crc_tmp ^ buf[i]) & 0xff] ^ (crc_tmp >> 8);
    }
    return ~crc_tmp;
}
#elif defined(SHA256_CHECK)

#endif