#include "script_engine.h"
#include "script_utils.h"
#include "../SDCard/sdmmc.h"

#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/dirent.h>

static const char TAG[] = "script_command";

int do_mkdir(char* cmd)
{
    char *dest = NULL;
    int opt_p = false;
    cmd_t *mycmd = parse(cmd, "p");

    char *path_dir = NULL;
    char *path_name = NULL;

    // for debug
    // print_cmd(mycmd);

    for(int i=0; i<mycmd->pair_num; i++)
    {
        if(!mystrcmp(mycmd->pair_table[i].opt, "-p"))
            opt_p = true;
        else
            dest = mycmd->pair_table[i].param;
    }
    if(dest)
    {
        if(mkdir(dest))
            ESP_LOGW(TAG, "mkdir error: %s", dest);
    }

    free(mycmd);
    return 0;
}

int do_cd(char* cmd)
{
    char *dest = NULL;
    int opt_p = false;
    cmd_t *mycmd = parse(cmd, "");

    char *path_dir = NULL;
    char *path_name = NULL;

    // for debug
    print_cmd(mycmd);

    if(mycmd->pair_num>1)
        ESP_LOGW(TAG, "command *cd* has more than one param");
    int i=0;
    while(i<mycmd->pair_num)
    {
        if(mycmd->pair_table[i].opt == NULL)
        {
            if(mycmd->pair_table[i].param)
                dest = mycmd->pair_table[i].param;
        }
        i++;
    }

    if(dest)
    {
        path_dir = mydirname(dest);
        path_name = mybasename(dest);
    }

    free(path_dir);
    free(mycmd);
    return 0;
}

int do_ls(char* cmd)
{
    char *dest = NULL;
    int opt_p = false;
    cmd_t *mycmd = parse(cmd, "");

    char *path_dir = NULL;
    char *path_name = NULL;

    // for debug
    print_cmd(mycmd);

    int i=0;
    while(i<mycmd->pair_num)
    {
        if(mycmd->pair_table[i].opt == NULL)
        {
            if(mycmd->pair_table[i].param)
                dest = mycmd->pair_table[i].param;
        }
        i++;
    }
    if(dest)
    {
        path_dir = mydirname(dest);
        path_name = mybasename(dest);
    }

    free(path_dir);
    free(mycmd);
    return 0;
}

int do_cp(char* cmd)
{
    char *src = NULL;
    char *dest = NULL;
    int opt_p = false;
    cmd_t *mycmd = parse(cmd, "");

    char *path_dir = NULL;
    char *path_name = NULL;

    // for debug
    print_cmd(mycmd);

    int i=0;
    while(i<mycmd->pair_num)
    {
        if(mycmd->pair_table[i].opt == NULL)
        {
            if(mycmd->pair_table[i].param)
            {
                if(dest == NULL)
                    dest = mycmd->pair_table[i].param;
                else
                    src = mycmd->pair_table[i].param;
            }
        }
        i++;
    }
    if(dest)
    {
        path_dir = mydirname(dest);
        path_name = mybasename(dest);
    }

    free(path_dir);
    free(mycmd);
    return 0;
}

int do_rm(char* cmd)
{
    char *dest = NULL;
    int opt_p = false;
    cmd_t *mycmd = parse(cmd, "");

    char *path_dir = NULL;
    char *path_name = NULL;
    // for debug
    print_cmd(mycmd);

    int i=0;
    while(i<mycmd->pair_num)
    {
        if(mycmd->pair_table[i].opt == NULL)
        {
            if(mycmd->pair_table[i].param)
                dest = mycmd->pair_table[i].param;
        }
        i++;
    }
    if(dest)
    {
        path_dir = mydirname(dest);
        path_name = mybasename(dest);
    }

    free(path_dir);
    free(mycmd);
    return 0;
}


int exec_cmd(char *cmd_str)
{
    char cmd[20];
    int num=0, ret=0;
    char *p = next_token(cmd_str, &num);
    memcpy(cmd, p, num);
    cmd[num] = '\0';
    // ESP_LOGI(TAG, "Command: %s", cmd);
    if(!mystrcmp(cmd, "mkdir"))
    {
        ret = do_mkdir(cmd_str);
    }
    else if(!mystrcmp(cmd, "cd"))
    {
        ret = do_cd(cmd_str);
    }
    else if(!mystrcmp(cmd, "ls"))
    {
        ret = do_ls(cmd_str);
    }
    else if(!mystrcmp(cmd, "cp"))
    {
        ret = do_cp(cmd_str);
    }
    else if(!mystrcmp(cmd, "rm"))
    {
        ret = do_rm(cmd_str);
    }
    return ret;
}
