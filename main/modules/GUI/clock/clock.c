#include "clock.h"

const char * const weekday[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "SAT", "Sun"};

char time_hm[20];
char time_s[20];
char time_ymd[20];
const char *time_wd = weekday[1];
static const char TAG[] = "clock";

static void clock_reflush(struct _lv_task_t *task);
esp_err_t ConfigTime(char *url);
void UpdateLabel(void);

// void pointer_clock(void)
// {
//     lv_style_init(&main_style);
//     lv_style_set_bg_opa(&main_style, LV_STATE_DEFAULT, LV_OPA_COVER);
//     lv_style_set_bg_color(&main_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

//     base_obj = lv_obj_create(lv_scr_act(), NULL);
//     lv_obj_set_pos(base_obj, 0, 0);
//     lv_obj_set_size(base_obj, LV_VER_RES_MAX, LV_VER_RES_MAX);

//     lv_obj_reset_style_list(base_obj, LV_OBJ_PART_MAIN);
//     lv_obj_add_style(base_obj, LV_OBJ_PART_MAIN, &main_style);

//     lv_obj_align(base_obj, NULL, LV_ALIGN_CENTER, 0, 0);

//     // text
//     lv_style_init(&label_style);
//     lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &lv_font_unscii_8);
//     lv_style_set_text_color(&label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

//     // shape and background
//     //lv_style_set_radius(&label_style, LV_STATE_DEFAULT, 100);
//     lv_style_set_bg_opa(&label_style, LV_STATE_DEFAULT, LV_OPA_COVER);
//     lv_style_set_bg_color(&label_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

//     // border
//     lv_style_set_border_color(&label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
//     lv_style_set_border_width(&label_style, LV_STATE_DEFAULT, 10);
//     lv_style_set_border_opa(&label_style, LV_STATE_DEFAULT, LV_OPA_COVER);

//     // Create Circle
//     clock_label = lv_label_create(base_obj, NULL);
//     lv_obj_reset_style_list(clock_label, LV_OBJ_PART_MAIN);
//     lv_obj_add_style(clock_label, LV_OBJ_PART_MAIN, &label_style);

//     lv_obj_set_width(clock_label, 200);
//     lv_obj_set_height(clock_label, 200);

//     lv_obj_align(clock_label, NULL, LV_ALIGN_CENTER, 0, 0);
// }

void digital_clock(char *url)
{
    ConfigTime(url);
    UpdateLabel();

    lv_style_init(&main_style);
    lv_style_set_bg_opa(&main_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&main_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    //
    lv_style_set_border_color(&main_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&main_style, LV_STATE_DEFAULT, 2);
    lv_style_set_border_opa(&main_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    //

    base_obj = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_pos(base_obj, 0, 0);
    lv_obj_set_size(base_obj, LV_HOR_RES_MAX, LV_VER_RES_MAX);

    lv_obj_reset_style_list(base_obj, LV_OBJ_PART_MAIN);
    lv_obj_add_style(base_obj, LV_OBJ_PART_MAIN, &main_style);

    // ===== hour & minute =====
    lv_style_init(&label_style_hm);

    // text
    lv_style_set_text_font(&label_style_hm, LV_STATE_DEFAULT, &lv_font_montserrat_48);
    lv_style_set_text_color(&label_style_hm, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // shape and background
    lv_style_set_bg_opa(&label_style_hm, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&label_style_hm, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    //
    lv_style_set_border_color(&label_style_hm, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&label_style_hm, LV_STATE_DEFAULT, 5);
    lv_style_set_border_opa(&label_style_hm, LV_STATE_DEFAULT, LV_OPA_COVER);
    //
    // Create Label
    clock_label_hm = lv_label_create(base_obj, NULL);
    lv_obj_reset_style_list(clock_label_hm, LV_OBJ_PART_MAIN);
    lv_obj_add_style(clock_label_hm, LV_OBJ_PART_MAIN, &label_style_hm);

    // lv_obj_set_pos(clock_label_hm, 0, 0);
    lv_label_set_long_mode(clock_label_hm, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(clock_label_hm, 140);
    lv_label_set_text_static(clock_label_hm, time_hm);

    // ===== second =====
    lv_style_init(&label_style_s);

    // text
    lv_style_set_text_font(&label_style_s, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&label_style_s, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // shape and background
    lv_style_set_bg_opa(&label_style_s, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&label_style_s, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    //
    lv_style_set_border_color(&label_style_s, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&label_style_s, LV_STATE_DEFAULT, 5);
    lv_style_set_border_opa(&label_style_s, LV_STATE_DEFAULT, LV_OPA_COVER);
    //
    // Create Label
    clock_label_s = lv_label_create(base_obj, NULL);
    lv_obj_reset_style_list(clock_label_s, LV_OBJ_PART_MAIN);
    lv_obj_add_style(clock_label_s, LV_OBJ_PART_MAIN, &label_style_s);

    // lv_obj_set_pos(clock_label_s, 0, 0);
    lv_label_set_text_static(clock_label_s, time_s);
    lv_label_set_long_mode(clock_label_s, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(clock_label_s, 50);

    // ===== year month day =====
    lv_style_init(&label_style_ymd);

    // text
    lv_style_set_text_font(&label_style_ymd, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&label_style_ymd, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // shape and background
    lv_style_set_bg_opa(&label_style_ymd, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&label_style_ymd, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    //
    lv_style_set_border_color(&label_style_ymd, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&label_style_ymd, LV_STATE_DEFAULT, 5);
    lv_style_set_border_opa(&label_style_ymd, LV_STATE_DEFAULT, LV_OPA_COVER);
    //
    // Create Label
    clock_label_ymd = lv_label_create(base_obj, NULL);
    lv_obj_reset_style_list(clock_label_ymd, LV_OBJ_PART_MAIN);
    lv_obj_add_style(clock_label_ymd, LV_OBJ_PART_MAIN, &label_style_ymd);

    lv_label_set_text_static(clock_label_ymd, time_ymd);
    lv_label_set_long_mode(clock_label_ymd, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(clock_label_ymd, 130);

    // ===== week day =====
    lv_style_init(&label_style_wd);

    // text
    lv_style_set_text_font(&label_style_wd, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&label_style_wd, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // shape and background
    lv_style_set_bg_opa(&label_style_wd, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&label_style_wd, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    //
    lv_style_set_border_color(&label_style_wd, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&label_style_wd, LV_STATE_DEFAULT, 5);
    lv_style_set_border_opa(&label_style_wd, LV_STATE_DEFAULT, LV_OPA_COVER);
    //
    // Create Label
    clock_label_wd = lv_label_create(base_obj, NULL);
    lv_obj_reset_style_list(clock_label_wd, LV_OBJ_PART_MAIN);
    lv_obj_add_style(clock_label_wd, LV_OBJ_PART_MAIN, &label_style_ymd);

    lv_label_set_text_static(clock_label_wd, time_wd);
    lv_label_set_long_mode(clock_label_wd, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(clock_label_wd, 50);

    // ==== print ====
    lv_obj_align(base_obj, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(clock_label_hm, NULL, LV_ALIGN_CENTER, -15, 0);
    lv_obj_align(clock_label_s, clock_label_hm, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, 0);
    lv_obj_align(clock_label_ymd, clock_label_hm, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_obj_align(clock_label_wd, clock_label_ymd, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

    lv_task_create(clock_reflush, 1000, LV_TASK_PRIO_MID, NULL);
}

static void clock_reflush(struct _lv_task_t *task)
{
    UpdateTime();
}

void UpdateTime(void)
{
    int upd_hm = false;
    int upd_day = false;

    sec++;
    if (sec % 60 == 0)
    {
        upd_hm = true;
        sec = 0;
        min++;
        if (min % 60 == 0)
        {
            min = 0;
            hour++;
            if (hour % 24 == 0)
            {
                hour = 0;
                day++;
                upd_day = true;
            }
        }
    }
    sprintf(time_s, "%02d", sec);
    lv_label_set_text_static(clock_label_s, NULL);
    if (upd_hm)
    {
        sprintf(time_hm, "%02d:%02d", hour, min);
        lv_label_set_text_static(clock_label_hm, NULL);
    }
    if(upd_day)
    {
        ConfigTime(NULL);
        UpdateLabel();
    }
}

void UpdateLabel(void)
{
    sprintf(time_s, "%02d", sec);
    sprintf(time_hm, "%02d:%02d", hour, min);
    sprintf(time_ymd, "%d/%d/%d", year, month, dayofmonth);
    time_wd = weekday[dayofweek];
}

static void http_cleanup(esp_http_client_handle_t client)
{
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

esp_err_t ConfigTime(char *url)
{
    esp_err_t err;
    char data[BUFFSIZE];
    static char update_url[50];

    if(url != NULL)
    {
        strcpy(update_url, url);
        strcat(update_url, "/time");
        ESP_LOGI(TAG, "Time Update URL: %s", update_url);
    }

    esp_http_client_config_t config = {
        .url = update_url,
        .timeout_ms = TIME_RECV_TIMEOUT};

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialise HTTP connection");
        return -1;
    }
    err = esp_http_client_open(client, 0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return -1;
    }
    esp_http_client_fetch_headers(client);
    int data_read = esp_http_client_read(client, data, BUFFSIZE);
    if (data_read < 0)
    {
        ESP_LOGE(TAG, "Error: SSL data read error");
        http_cleanup(client);
        return -1;
    }
    else if (data_read > 0)
    {
        data[data_read] = '\0';
        ESP_LOGI(TAG, "Time Update: %s", data);
        sscanf(data, "%d:%d:%d|%d/%d/%d|%d|%d", &hour, &min, &sec, &year, &month, &dayofmonth, &day, &dayofweek);
        http_cleanup(client);
        return 0;
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
        }
        if (esp_http_client_is_complete_data_received(client) == true)
        {
            ESP_LOGI(TAG, "Connection closed");
        }
        http_cleanup(client);
        return -1;
    }
}
