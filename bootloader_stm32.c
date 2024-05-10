
#include "./IAP/iap.h"
#include "bootloader_stm32.h"
#include "md5.h"
//因为是写入ROM中，所以要用到memcpy读取ROM中数据
#include <string.h>

typedef struct bootloader_header_t
{
    uint8_t MD5[16];    /* MD5值校验 */
    uint32_t ih_crc;  /* CRC32 */
    uint32_t ih_size;   /* 升级文件大小 */
    uint32_t ih_flag;   /* 升级标志位 */
    uint32_t reserve;   /* 预留标志位 */
} bootloader_header_t;

/**
 * @brief bootloader升级程序入口，检查flash中存储的升级程序地址，跳转到升级程序执
 * @param bootloader  bootloader结构体指针
 * @return int 0：升级成功，1：升级失败
 */
int bootloader_main(bootloader_t *bootloader)
{
    bootloader_header_t header;
    uint8_t md5[16];
    uint32_t update_file_size;
    uint32_t flash_base;
    uint32_t app_base;
    uint8_t update_buf[512];

    if ( 0x5A5A5A5A != header.ih_flag)
    {
        //当前版本代码已经是最新版本，不执行升级程序
        return 1;
    }
    
    //读取flash中存储的升级程序地址
    bootloader->bootloader_read(bootloader->srcaddr, (uint32_t)&header, sizeof(header));

    //如果有升级标志，则搬运flash中内容升级
    do
    {
        if(update_file_size>sizeof(update_buf))
        {
            bootloader->bootloader_read((uint32_t)update_buf, flash_base, sizeof(update_buf));
            flash_base += sizeof(update_buf);
            //写升级版本到当前ROM中
            bootloader->rom_write(bootloader->romaddr, bootloader->srcaddr + sizeof(header), header.ih_size);
            app_base += sizeof(update_buf);
        }
        else{
            bootloader->bootloader_read((uint32_t)update_buf, flash_base,update_file_size);
            //写升级版本到当前ROM中
            bootloader->rom_write(bootloader->romaddr, bootloader->srcaddr + sizeof(header), header.ih_size);
            printf("\r\n *** file is write , size_t is 0x%x *** \r\n", update_file_size);
            break;
        }
        //printf("\r\n\r\nnumber is 0x%x\r\n\r\n", update_file_size);
        update_file_size -= sizeof(update_buf);
    }while(1);

    MD5(bootloader->romaddr, header.ih_size, md5, bootloader->rom_read);

    //校验MD5值
    if(0 != memcmp(header.MD5, md5, 16))
    {
        //MD5值校验失败，不执行升级程序
        return 1;
    }
    printf("\r\n *** file is update success *** \r\n");
    return 0;

}










