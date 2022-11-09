#ifndef _SCRIPT_ENGINE_H
#define _SCRIPT_ENGINE_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define false 0
#define true 1


// if result of commands write back to server or just write through log
// #define INTERACTIVE

#ifdef INTERACTIVE

#else
#define LOGI(tag, fmt, args...)  ESP_LOGI(tag, fmt, ## args)
#endif


#define BUFFSIZE 100
static const char TAG[] = "script";
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


cmd_t* parse(char *str, char *optstr);  // use malloc
char* next_token(char *str, int *num);

#endif