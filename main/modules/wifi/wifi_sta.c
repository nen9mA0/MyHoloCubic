#include "wifi_sta.h"

char url[50];
static const char TAG[] = "wifi_sta";

uint8_t chr2num(char a)
{
    if(a >= 'A' && a <= 'F')
        return 10 + (a-'A');
    else if(a >= 'a' && a <= 'f')
        return 10 + (a-'a');
    else if(a >= '0' && a <= '9')
        return a-'0';
    else
    {
        ESP_LOGE(TAG, "chr2num err: Get char: %c", a);
        return 255;
    }
}

uint8_t *hex2bin(char* str, uint8_t *buf)
{
    uint8_t h, l;
    for(int i=0; i<6; i++)
    {
        h = chr2num(str[2*i]);
        l = chr2num(str[2*i+1]);
        buf[i] = h*16+l;
    }
    return buf;
}

esp_err_t SetMac(void)
{
    char mac_str[13];
    uint8_t mac[6];
    size_t num;

    mac_str[12] = '\0';
    ESP_LOGI(TAG, "Setting Mac");

    FILE *fp = fopen(MOUNT_POINT"/config/mac.txt", "r");
    if(fp == NULL)
    {
        ESP_LOGE(TAG, "Fopen mac.txt err");
        return -1;
    }
    else
    {
        num = fread(mac_str, 1, 12, fp);
        if(num == 12)
        {
            hex2bin(mac_str, mac);
            esp_base_mac_addr_set(mac);
            ESP_LOGI(TAG, "set_mac: Set Mac Address: %s", mac_str);
        }
        else
        {
            ESP_LOGE(TAG, "Reading Bytes In mac.txt: %s", mac_str);
        }
    }
    fclose(fp);
    return 0;
}

esp_err_t GetWifiId(char *ssid, char *passwd)
{
    FILE *fp = fopen(MOUNT_POINT"/config/wifi.txt", "r");
    if(fp == NULL)
    {
        ESP_LOGE(TAG, "Fopen wifi.txt err");
        return -1;
    }
    else
    {
        fscanf(fp, "%s", ssid);             // may cause stack overflow here
        fscanf(fp, "%s", passwd);
        ESP_LOGI(TAG, "Get SSID: %s", ssid);
        ESP_LOGI(TAG, "Get PASSWD: %s", passwd);
    }
    fclose(fp);
    return 0;
}

esp_err_t GetUrl(char* url)
{
    FILE *fp = fopen(MOUNT_POINT"/config/url.txt", "r");
    if(fp == NULL)
    {
        ESP_LOGE(TAG, "Fopen url.txt err");
        return -1;
    }
    else
    {
        fscanf(fp, "%s", url);
    }
    fclose(fp);
    return 0;
}

/*
    wifi init must be called after sdcard init and NVS init
*/
esp_err_t wifi_init(void)
{
    char ssid[30] = {0};
    char passwd[30] = {0};

    esp_err_t ret = 0;

    ret = SetMac();
    if(!ret)
    {
        ret = GetWifiId(ssid, passwd);
        if(!ret)
        {
            ret = GetUrl(url);
            if(!ret)
            {
                ESP_ERROR_CHECK(esp_netif_init());
                ESP_ERROR_CHECK(esp_event_loop_create_default());

                /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
                * Read "Establishing Wi-Fi or Ethernet Connection" section in
                * examples/protocols/README.md for more information about this function.
                */
                ESP_ERROR_CHECK(example_connect(ssid, passwd));
                esp_wifi_set_ps(WIFI_PS_NONE);
                wifi_disconnected = false;
                return 0;
            }
        }
    }
    return -1;
}
