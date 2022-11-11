#include "script_engine.h"

#define BUFSIZE 50

static const char TAG[] = "script_interactive";

int UpdateScript(char *url)
{
    esp_err_t err=0;
    char buf[2][BUFSIZE+1] = {0};
    int index=0, i=0, len=0, data_read=0;
    char *tmp_buf;
    char *current_buf = buf[index];
    int ret = -1;
 
    memset(path, 0, PATH_LEN+1);
    strcpy(path, MOUNT_POINT);

    esp_http_client_handle_t handle = ClientInit(url, "/upg_dev_script");
    if(handle)
    {
        err = ClientOpen(handle);
        if(err>=0)
        {
            while(1)
            {
                if(i<len)
                {
                    if(current_buf[i]=='\n' || current_buf[i]=='\0')
                    {
                        current_buf[i] = '\0';
                        ESP_LOGI(TAG, "cmd: %s", current_buf);
                        ret = exec_cmd(current_buf);

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
                    data_read = ClientRead(handle, &current_buf[len], BUFSIZE-len);
                    if (data_read > 0)
                    {
                        len += data_read;
                    }
                    else if(data_read == 0)
                    {
                        if(len>0)
                        {
                            ESP_LOGI(TAG, "cmd: %s", current_buf);
                            ret = exec_cmd(current_buf);
                        }
                        break;
                    }
                    else
                    {
                        return -1;
                    }
                }
            }
            return ret;
        }
    }
    return -1;
}
