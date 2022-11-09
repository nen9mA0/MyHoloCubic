#include "myhttp.h"

static char TAG[] = "myhttp";

void ClientCleanup(esp_http_client_handle_t client)
{
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

esp_http_client_handle_t ClientInit(char *url, char *path)
{
    char url_tmp[URL_LENGTH];

    if(!url)
    {
        ESP_LOGE(TAG, "Empty URL");
        return NULL;
    }
    else
    {
        strcpy(url_tmp, url);
        strcat(url_tmp, path);
        ESP_LOGI(TAG, "URL: %s", url_tmp);
    }

    esp_http_client_config_t config = {
        .url = url_tmp,
        .timeout_ms = TIME_RECV_TIMEOUT};

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialise HTTP connection");
        return NULL;
    }
    return client;
}

esp_err_t ClientOpen(esp_http_client_handle_t client)
{
    esp_err_t err;
    err = esp_http_client_open(client, 0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return -1;
    }
    esp_http_client_fetch_headers(client);
    return err;
}

int ClientRead(esp_http_client_handle_t client, char* data, int len)
{
    int err;
    int data_read = esp_http_client_read(client, data, len);
    if (data_read < 0)
    {
        ESP_LOGE(TAG, "Error: SSL data read error");
        ClientCleanup(client);
        return -1;
    }
    else if (data_read > 0)
    {
        data[data_read] = '\0';
        return data_read;
    }
    else
    {
        /*
            * As esp_http_client_read never returns negative error code, we rely on
            * `errno` to check for underlying transport connectivity closure if any
            */
        if (errno == ECONNRESET || errno == ENOTCONN)
        {
            ESP_LOGE(TAG, "Connection closed, errno = %d", errno);
            ClientCleanup(client);
            err = -1;
        }
        if (esp_http_client_is_complete_data_received(client) == true)
        {
            ESP_LOGI(TAG, "Connection End");
            err = 0;
        }
        return err;
    }
}