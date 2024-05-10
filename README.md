# bootloader-stm32


#### 使用说明

1.  本项目只涉及一个结构体和一个接口函数
用户需要提供结构体中所需参数，然后调用接口函数，即可完成升级
typedef struct bootloader
{
    unsigned int romaddr;           //romaddr   rom中当前版本代码存放的地址
    unsigned int srcaddr;           //srcaddr   升级包存放的地址
    BUFCOPY rom_write;          //rom_write  差分升级包写入ROM回调函数，例如：iap_write_appbin等
    BUFCOPY rom_read;           //rom_read   升级包读取ROM回调函数，例如：memcpy等
    BUFCOPY bootloader_read;    //bootloader_read   差分升级包读取flash回调函数，例如：norflash_read，W25QXX_Read，memcpy等
    /* data */
}bootloader_t; 

2. 升级接口，将升级包写入给定ROM地址，其中完成ROM中APP的MD5校验
返回值：0-成功（可手动跳转到APP运行），其他-失败
int bootloader_main(bootloader_t *bootloader);


