#include "script_engine.h"

cmd_t* parse(char *str, char *optstr)
{
    char *cmd_str;
    char *p=str, *prev_p=str, *end_p, *tmp;
    int num=0, pair_num=0, length=0, i;
    int have_param = false;

    cmd_t *cmd=NULL;

    cmd_str = next_token(p, &num);
    if(cmd_str)
    {
        p += num;

        // first scan, to determine number of parameters
        while(p = next_token(p, &num))
        {
            if(p[0] == '-')             // -opt
            {
                have_param = false;
                tmp = strchr(optstr, p[1]);
                if(tmp)
                {
                    pair_num++;
                    if(tmp[1] == ':')   // -opt param
                    {
                        have_param = true;
                    }
                }
            }
            else
            {
                if(have_param)
                    have_param = false;
                else
                    pair_num++;
            }
            p += num;
            prev_p = p;
        }
        length = prev_p-cmd_str;

        i = pair_num;
        if(0 == i)
            i++;
        cmd = malloc( sizeof(cmd_t)+(i-1)*sizeof(pair_t) );
        memset(cmd, 0, sizeof(cmd_t)+(i-1)*sizeof(pair_t));

        cmd->cmd = cmd_str;
        cmd->length = length;
        cmd->pair_num = pair_num;
        p = next_token(cmd_str, &num);  // skip command string
        p[num] = '\0';
        p += num+1;

        i = -1;
        end_p = cmd_str+length;
        have_param = false;

        if(p < end_p)
        {
            while(p=next_token(p, &num))
            {
                if(p[0] == '-')             // -opt
                {
                    have_param = false;
                    tmp = strchr(optstr, p[1]);
                    if(tmp)
                    {
                        p[num] = '\0';
                        i++;
                        cmd->pair_table[i].opt = p;
                        if(tmp[1] == ':')   // -opt param
                        {
                            have_param = true;
                        }
                    }
                }
                else
                {
                    if(have_param)
                    {
                        have_param = false;
                        p[num] = '\0';
                        cmd->pair_table[i].param = p;
                    }
                    else
                    {
                        i++;
                        p[num] = '\0';
                        cmd->pair_table[i].param = p;
                    }
                }
                p += num+1;                 // because previous program set p[num] to \0, so we should add (num+1)
                if(p >= end_p)
                    break;
            }
        }
    }
    return cmd;
}

char* next_token(char *str, int *num)
{
    char *tmp;
    char *q = NULL;
    char *p = str;
    *num = 0;
    while( (*p!='\n') && (*p!='\0') )
    {
        if(isspace(*p))
        {
            if(q)           // if we have get a token
            {
                break;
            }
            else            // else we jumpout the space at the beginning
            {
                p++;
                while(isspace(*p))
                    p++;
            }
        }
        else
        {
            tmp = strchr(symbol, *p);
            if(tmp)
            {
                if(!q)
                    q = p;
            }
            p++;
        }
    }
    *num = p-q;
    return q;
}
