#ifndef _MYHTTP_H
#define _MYHTTP_H

#include <string.h>

#include "esp_log.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "errno.h"

#include "../myconfig.h"

#define TIME_RECV_TIMEOUT 5000


esp_http_client_handle_t ClientInit(char *url, char *path);
esp_err_t ClientOpen(esp_http_client_handle_t client);
int ClientRead(esp_http_client_handle_t client, char* data, int len);

void ClientCleanup(esp_http_client_handle_t client);

#endif