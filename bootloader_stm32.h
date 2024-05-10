#ifndef BOOTLOADER_STM32
#define BOOTLOADER_STM32

typedef void (*BUFCOPY)(uint32_t dst_addr, uint32_t src_addr, uint32_t len);
 

typedef struct bootloader
{
    uint32_t romaddr;           //romaddr   rom中当前版本代码存放的地址
    uint32_t srcaddr;           //srcaddr   升级包存放的地址
    BUFCOPY rom_write;          //rom_write  差分升级包写入ROM回调函数，例如：iap_write_appbin等
    BUFCOPY rom_read;           //rom_read   升级包读取ROM回调函数，例如：memcpy等
    BUFCOPY bootloader_read;    //bootloader_read   差分升级包读取flash回调函数，例如：norflash_read，W25QXX_Read，memcpy等
    /* data */
}bootloader_t; 

int bootloader_main(bootloader_t *bootloader);



#endif // BOOTLOADER_STM32
