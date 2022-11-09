#include <stdio.h>
#include "../script_engine/script_engine.h"

void PrintCmd(cmd_t *p)
{
    printf("%s\n", p->cmd);
    for(int i=0; i<p->pair_num; i++)
    {
        if(p->pair_table[i].opt != NULL)
            printf("\topt: %s", p->pair_table[i].opt);
        if(p->pair_table[i].param != NULL)
            printf("\tparam: %s", p->pair_table[i].param);
        printf("\n");
    }
}

int main(void)
{
    char input[50];

    while(1)
    {
        gets(input);
        if(strcmp(input, "q") == 0)
            break;
        cmd_t *p = parse(input, "p");
        PrintCmd(p);
    }
}