#ifndef _CLOCK_H
#define _CLOCK_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "errno.h"

/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#define TIME_RECV_TIMEOUT 5000
#define BUFFSIZE 30

static lv_style_t main_style;
static lv_style_t label_style_hm;
static lv_style_t label_style_s;
static lv_style_t label_style_ymd;
static lv_style_t label_style_wd;
static lv_obj_t *base_obj = NULL;
static lv_obj_t *clock_label_hm = NULL;
static lv_obj_t *clock_label_s = NULL;
static lv_obj_t *clock_label_ymd = NULL;
static lv_obj_t *clock_label_wd = NULL;

static unsigned int year = 2022, day = 18;
static unsigned int sec = 0, min = 0, hour = 0;
static unsigned int dayofmonth = 18, dayofweek = 1, month = 1;

// void pointer_clock(void);
void digital_clock(char *url);
void UpdateTime(void);

#endif