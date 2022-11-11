#ifndef _SCRIPT_ENGINE_H
#define _SCRIPT_ENGINE_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../http/myhttp.h"
#include "../SDCard/sdmmc.h"

#define false 0
#define true 1

#define PATH_LEN 50

// if result of commands write back to server or just write through log
// #define INTERACTIVE

#ifdef INTERACTIVE

#else
#define LOGI(tag, fmt, args...)  ESP_LOGI(tag, fmt, ## args)
#endif

static char path[PATH_LEN+1];
static const char symbol[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_.\"\'";

typedef struct _pair_t
{
    char *opt;
    char *param;
} pair_t;

typedef struct _cmd_t
{
    char *cmd;
    int pair_num;
    int length;
    pair_t pair_table[1];
} cmd_t;

int exec_cmd(char *cmd_str);

int UpdateScript(char *url);
cmd_t* parse(char *str, char *optstr);  // use malloc
char* next_token(char *str, int *num);
void print_cmd(cmd_t *p);

#endif