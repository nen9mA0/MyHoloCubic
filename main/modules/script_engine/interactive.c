#include "script_engine.h"

static void http_cleanup(esp_http_client_handle_t client)
{
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

esp_err_t UpdateScript(char *url)
{
    esp_err_t err;
    char data[BUFFSIZE];
 
    esp_http_client_handle_t handle = ClientInit(url, "/upg_dev_script");
    if(handle)
    {
        err = ClientOpen(handle);
        while(1)
        {
            int data_read = ClientRead(handle, data, BUFFSIZE);
            if (data_read > 0)
            {

            }
            else if(data_read == 0)
            {
                break;
            }
            else
            {
                return -1;
            }
        }
    }
}
