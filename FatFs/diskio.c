/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"		/* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */
#include "w25qxx.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM 0 /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC 1 /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB 2 /* Example: Map USB MSD to physical drive 2 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	u16 result;
	switch (pdrv)
	{
	case DEV_RAM:
		result = W25QXX_ReadID();
		if (result == W25Q64)
			stat = RES_OK;
			
		// translate the reslut code here
		return stat;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	u16 result;
	switch (pdrv)
	{
	case DEV_RAM:
		W25QXX_Init();
		result = W25QXX_ReadID();
		if (result == W25Q64)
			stat = RES_OK;
		// translate the reslut code here
		return stat;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
	BYTE pdrv,	  /* Physical drive nmuber to identify the drive */
	BYTE *buff,	  /* Data buffer to store read data */
	LBA_t sector, /* Start sector in LBA */
	UINT count	  /* Number of sectors to read */
)
{
	switch (pdrv)
	{
	case DEV_RAM:
		// translate the arguments here
		sector+=512; 
		W25QXX_Read(buff, sector * 4096, count * 4096);

		// translate the reslut code here

	}

	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
	BYTE pdrv,		  /* Physical drive nmuber to identify the drive */
	const BYTE *buff, /* Data to be written */
	LBA_t sector,	  /* Start sector in LBA */
	UINT count		  /* Number of sectors to write */
)
{

	switch (pdrv)
	{
	case DEV_RAM:
		// translate the arguments here
		sector+=512; 
		W25QXX_Erase_Sector(sector * 4096);
		W25QXX_Write((u8*)buff, sector * 4096, count * 4096);

		// translate the reslut code here

	}

	return RES_OK;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
	BYTE pdrv, /* Physical drive nmuber (0..) */
	BYTE cmd,  /* Control code */
	void *buff /* Buffer to send/receive control data */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv)
	{
	case DEV_RAM:
	
		// Process of the command for the RAM drive
		switch (cmd)
		{
		/* 扇区数量：1536*4096/1024/1024=6(MB) */
		case GET_SECTOR_COUNT:
			*(DWORD *)buff = 1536;
			break;
		/* 扇区大小 */
		case GET_SECTOR_SIZE:
			*(WORD *)buff = 4096;
			break;
		/* 同时擦除扇区个数 */
		case GET_BLOCK_SIZE:
			*(DWORD *)buff = 1;
			break;
		}
		status = RES_OK;
		break;
	}

	return status;
}

DWORD get_fattime(void)
{
	return ((2021 - 1980) << 25) | (6 << 21) | (2 << 16) | (11 << 11) | (35 << 5) | (2 << 0);
}

