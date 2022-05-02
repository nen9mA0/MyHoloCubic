基于lv_port_esp32进行修改

加入一些feature

#### 镜像显示

##### lv_hal_disp.h

lv_disp_drv_t 结构体将 rotated 成员改为3位

lv_disp_rot_t 加入成员 LV_DISP_MIRROR

##### lv_refr.c

加入lv_refr_vdb_mirror函数处理镜像请求

lv_refr_vdb_rotate 加入处理 LV_DISP_MIRROR 的判断