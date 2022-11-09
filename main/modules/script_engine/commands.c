#include "script_engine.h"
#include "script_utils.h"
#include "../SDCard/sdmmc.h"

int exec_cmd(char *cmd_str)
{
    char cmd[20];
    int num=0, ret;
    char *p = next_token(cmd_str, &num);
    memcpy(cmd, p, num);
    cmd[num] = '\0';
    if(!strcmp(cmd, "mkdir"))
    {
        ret = do_mkdir(cmd);
    }
    else if(!strcmp(cmd, "cd"))
    {

    }
    else if(!strcmp(cmd, "ls"))
    {

    }
    else if(!strcmp(cmd, "cp"))
    {

    }
    else if(!strcmp(cmd, "rm"))
    {

    }
    free(cmd_str);
    return ret;
}

int do_mkdir(char* cmd)
{
    char *dest = NULL;
    int opt_p = false;
    cmd_t *mycmd = parse(cmd, "p");
    for(int i=0; i<mycmd->pair_num; i++)
    {
        if(!strcmp(mycmd->pair_table[i].opt, "-p"))
            opt_p = true;
        else
            dest = mycmd->pair_table[i].param;
    }
    if(dest)
    {
        char *path_dir = dirname(dest);
        char *path_name = basename(dest);
    }
    return 0;
}

int do_cd(char* cmd)
{

}

int do_ls(char* cmd)
{

}

int do_cp(char* cmd)
{

}

int do_rm(char* cmd)
{

}
