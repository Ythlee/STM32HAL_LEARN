#include "sys.h"
#include "ff.h"
#include "stdio.h"
#include "string.h"

const u8 TEXT_Buffer[] = {"杨灿 STM32 FATFS 测试"};

#define SIZE sizeof(TEXT_Buffer)
u8 RESIVE_Buffer[SIZE];
BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
FATFS FatFs;
FIL fil;
FRESULT fr, res;

FATFS fs;
FIL fnew;
FRESULT res_flash;
UINT fnum;
char fpath[100];
char readbuffer[512];

void fatfs_write_and_read(void)
{
    u32 bw, br;
    res = f_mount(&FatFs, "", 1);
    if (res == FR_OK)
        printf("文件挂在成功\r\n");
    else
    {
        printf("问价挂在失败\r\n");
        res = f_mkfs("", 0, work, sizeof work);
        if (res == FR_OK)
        {
            printf("f_mkfs成功\r\n");
            fr = f_open(&fil, "message.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
        }
        else
            printf("f_mkfs失败代码为：%d\r\n", res);
    }
    fr = f_open(&fil, "message.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK)
        printf("打开文件成功\r\n");
    else
    {
        printf("打开文件失败代码为：%d\r\n", fr);
    }

    res = f_write(&fil, TEXT_Buffer, SIZE, &bw);
    if (res != FR_OK)
        printf("f_write失败代码为：%d\r\n", res);
    f_close(&fil);

    fr = f_open(&fil, "message.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
    f_read(&fil, RESIVE_Buffer, SIZE, &br);
    printf("文件内容为：");
    printf("%s\r\n", RESIVE_Buffer);
    f_close(&fil);
}

FRESULT miscellaneous(void)
{
//    DIR dir;
    FATFS *pfs;
    DWORD fre_clust, fre_sect, tot_sect;

      printf("\n*************** 设备信息获取 ***************\r\n");
    /* Get volume information and free clusters of drive 1 */
    res = f_getfree("", &fre_clust, &pfs);

    /* Get total sectors and free sectors */
    tot_sect = (pfs->n_fatent - 2) * pfs->csize;
    fre_sect = fre_clust * pfs->csize;
     printf("》设备总空间：%10lu KB\n》可用空间：  %10lu KB\n", tot_sect *4, fre_sect *4);

    res_flash = f_open(&fnew, "message.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
    if (res_flash == FR_OK)
    {
        res_flash = f_lseek(&fnew, f_size(&fnew));
        if (res_flash == FR_OK)
        {
            f_printf(&fnew,"\n在原来文件新添加一行内容\n");
            f_printf(&fnew,"》设备总空间：%10lu KB\n》可用空间：  %10lu KB\n", tot_sect *4, fre_sect *4);
            res_flash = f_lseek(&fnew, 0);
            res_flash = f_read(&fnew, readbuffer, f_size(&fnew), &fnum);
            if (res_flash == FR_OK)
            {
                printf("》文件内容：\n%s\n",readbuffer);
            }
        }
    }
    f_close(&fnew);
	return FR_OK;
}


