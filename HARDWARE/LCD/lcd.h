#ifndef __LCD_H
#define __LCD_H

#include "sys.h"

extern SRAM_HandleTypeDef TFTSRAM_Handler;

typedef struct
{
    u16 width;
    u16 height;
    u16 id;
    u8 dir;
    u16 wramcmd;
    u16 setxcmd;
    u16 setycmd;
} _lcd_dev;

extern _lcd_dev lcddev;
extern u32 POINT_COLOR;
extern u32 BACK_COLOR;

#define LCD_LED PBout(0) //LCD背光 PB0

typedef struct
{
    vu16 LCD_REG;       //命令
    vu16 LCD_RAM;       //数据
} LCD_Typedef;

#define LCD_BASE ((u32)(0x6C000000) | 0x000007FE)
#define LCD ((LCD_Typedef *)LCD_BASE)

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40
#define BRRED 			 0XFC07
#define GRAY  			 0X8430

#define DARKBLUR         0X01CF
#define LIGHTBLUE      	 0X7D7C
#define GRAYBLUE       	 0X5458

#define LIGHTGREEN     	 0X841F
//#define LIGHTGRAY        0XEF5B
#define LGRAY 			 0XC618

#define LGRAYBLUE        0XA651
#define LBBLUE           0X2B12


void LCD_Init(void);
void LCD_DrawPoint(u16 x, u16 y);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1,u16 x2, u16 y2);
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r);
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);
void show_bmp(u16 * color);

#endif // !__LCD_H
