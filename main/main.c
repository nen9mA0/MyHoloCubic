/* LVGL Example project
 *
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "esp_err.h"

// ==== sdcard ====
#include "sdmmc.h"
#include <sys/unistd.h>
#include <sys/stat.h>

// ====  wifi  ====
#include "nvs_flash.h"
#include "wifi_sta.h"

// ==== ota ====
#include "ota.h"

// ====  mpu ====
#include "mpu.h"

// ==== input ====
#include "input.h"

// ==== lvgl ====
#include "lvgl.h"
#include "lvgl_helpers.h"

// ==== link list ====
#include "lvgl/src/lv_misc/lv_ll.h"


#include "clock.h"
#include "script_engine.h"
/*********************
 *      DEFINES
 *********************/
#define LV_TICK_PERIOD_MS 1

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_tick_task(void *arg);
static void guiTask(void *arg);
// static void mpu_test_task(void *arg);
// static void wifi_connect_task(void *arg);

const char TAG[] = "main";

const char gui_task_str[] = "gui";
const char clock_tick_task_str[] = "clock_tick";
const char wifi_task_str[] = "wifi";
const char gpio_task_str[] = "gpio";

bool ota_done = false;
bool wifi_disconnected = true;
lv_ll_t ll_task;
lv_ll_node_t *head;


esp_err_t init(int *err_bit)
{
    esp_err_t err;

    // sd card init
    err = sdmmc_init();
    if(err)
        *err_bit |= 0x1;

    // nvs init for wifi
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
    if(err)
        *err_bit |= 0x2;


    err = wifi_init();
    if(err)
        *err_bit |= 0x4;

    // mpu serial init
    err = mpu_serial_init();
    if(err)
        *err_bit |= 0x8;

    // input init
    // input_init();

    return err;
}



void app_main()
{
    esp_err_t ret;
    int err_bit;
    TaskHandle_t handle;
    lv_ll_node_t *tmp;

    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */

    ret = init(&err_bit);

    if( (err_bit & 0x4) == 0)      // if wifi connected
    {
        ret = ota_start(url);
        ret = UpdateScript(url);
    }

    xTaskCreate(guiTask, gui_task_str, 2048*2, NULL, 5, NULL);
    // input button invalid because circuit is wrong
    // xTaskCreate(gpio_task, gpio_task_str, 2048, NULL, 5, NULL);
}

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;
lv_disp_t *disp;

void cb_mirror_framebuffer(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
    disp_driver_flush(drv, area, color_map);
}

static void guiTask(void *arg)
{
    // lv_obj_t *old_scr = NULL;
    // lv_obj_t *new_scr = NULL;
    (void) arg;
    xGuiSemaphore = xSemaphoreCreateMutex();

    // lvgl init
    lv_init();
    /* Initialize SPI or I2C bus used by the drivers */
    lvgl_driver_init();

    ESP_LOGI(TAG, "Display Size: %d  %d", LV_VER_RES_MAX, LV_HOR_RES_MAX);

    lv_color_t* buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);

    /* Use double buffered when not working with monochrome displays */
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    lv_color_t* buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2 != NULL);
#else
    static lv_color_t *buf2 = NULL;
#endif

    static lv_disp_buf_t disp_buf;

    uint32_t size_in_px = DISP_BUF_SIZE;

#if defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_IL3820         \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_JD79653A    \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_UC8151D     \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_SSD1306

    /* Actual size in pixels, not bytes. */
    size_in_px *= 8;
#endif

    /* Initialize the working buffer depending on the selected display.
     * NOTE: buf2 == NULL when using monochrome displays. */
    lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = cb_mirror_framebuffer;
    disp_drv.rotated = LV_DISP_MIRROR;
    disp_drv.sw_rotate = 1;

    /* When using a monochrome display we need to register the callbacks:
     * - rounder_cb
     * - set_px_cb */
#ifdef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    disp_drv.rounder_cb = disp_driver_rounder;
    disp_drv.set_px_cb = disp_driver_set_px;
#endif

    disp_drv.buffer = &disp_buf;
    disp = lv_disp_drv_register(&disp_drv);

    /* Register an input device when enabled on the menuconfig */
#if CONFIG_LV_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touch_driver_read;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
#endif

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    /* Create the demo application */
    digital_clock(url);

    while (1) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));
        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }

    /* A task should NEVER return */
    free(buf1);
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    free(buf2);
#endif
    vTaskDelete(NULL);
}


static void lv_tick_task(void *arg)
{
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}


// static void wifi_connect_task(void *arg)
// {
//     esp_err_t err;
//     lv_ll_node_t *node;

//     while(wifi_disconnected)
//     {
//         node = head;
//         while(_lv_ll_get_next(&ll_task, node) != NULL)
//         {
//             node = _lv_ll_get_next(&ll_task, node);
//             if(node != arg)
//             {
//                 ESP_LOGI(TAG, "Suspend handle %x", *node);
//                 vTaskSuspend(*(TaskHandle_t *)node);
//             }
//             else
//             {
//                 ESP_LOGI(TAG, "handle equal");
//             }
//         }

//         // wifi init
//         err = wifi_init();
//         if(!err)        // if wifi connected
//         {
//             wifi_disconnected = false;
//             if(!ota_done)
//             {
//                 err = ota_start(url);
//                 ota_done = true;        // whether ota is successful or not, mark it as success.
//                 if(!err)
//                 {
//                     break;
//                 }
//             }

//         }
//         node = head;
//         while(_lv_ll_get_next(&ll_task, node) != NULL)
//         {
//             node = _lv_ll_get_next(&ll_task, node);
//             if(node != arg)
//             {
//                 ESP_LOGI(TAG, "Resume handle %x", *node);
//                 vTaskResume(*(TaskHandle_t *)node);
//             }
//             else
//             {
//                 ESP_LOGI(TAG, "handle equal");
//             }
//         }
//         vTaskDelay(10000 / portTICK_RATE_MS);
//     }
//     vTaskDelete(NULL);
// }

// static void mpu_test_task(void *arg)
// {
//     int len = 0;
//     uint8_t *data;

//     // waiting for calibration
//     vTaskDelay(3000 / portTICK_RATE_MS);

//     mpu_set_output(0x53);
//     vTaskDelay(20 / portTICK_RATE_MS);
//     while(1)
//     {
//         data = mpu_get_data(&len);
//         for(int i=0; i<len; i++)
//             printf("%X ", data[i]);
//         printf("\n");

//         vTaskDelay(500 / portTICK_RATE_MS);
//     }
// }
