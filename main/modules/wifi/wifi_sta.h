#ifndef _WIFI_STA
#define _WIFI_STA

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"


// for importing MOUNT_POINT
#include "sdmmc.h"

extern char url[50];

esp_err_t wifi_init(void);

#endif