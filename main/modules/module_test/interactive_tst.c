#include <stdio.h>
#include <string.h>

#define BUFSIZE 15

int main()
{
    char buf[2][BUFSIZE+1] = {0};
    int index=0, i=0, len=0, data_read=0;
    char *tmp_buf;
    char *current_buf = buf[index];
    int ret = -1;

    FILE *fp = fopen("script.txt", "r");

    while(1)
    {
        if(i<len)
        {
            if(current_buf[i]=='\n' || current_buf[i]=='\0' || current_buf[i]=='\r')
            {
                current_buf[i] = '\0';
                // ret = exec_cmd(current_buf);
                printf("%s\n", current_buf);

                tmp_buf = current_buf;
                index = index ^ 1;
                current_buf = buf[index];                   // switch buffer
                memcpy(current_buf, &tmp_buf[i+1], len-i-1);    // copy reserve string to new buffer
                len -= i+1;
                i = 0;
            }
            else
                i++;
        }
        else
        {
            // data_read = fread(handle, &current_buf[len], BUFSIZE-len);
            data_read = fread(&current_buf[len], 1,  BUFSIZE-len, fp);
            if (data_read > 0)
            {
                current_buf[len+data_read] = '\0';
                len += data_read;
            }
            else if(data_read == 0)
            {
                if(len>0)
                    printf("%s\n", current_buf);
                break;
            }
            else
            {
                return -1;
            }
        }
    }
    return 0;
}