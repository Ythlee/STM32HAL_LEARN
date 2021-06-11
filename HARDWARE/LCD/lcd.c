#include "lcd.h"
#include "delay.h"
#include "stdio.h"
SRAM_HandleTypeDef TFTSRAM_Handler;

_lcd_dev lcddev;

u32 POINT_COLOR = 0x0000; //画笔颜色
u32 BACK_COLOR = 0xFFFF;  //背景颜色

//写寄存器函数
static void LCD_WR_REG(u16 regval)
{
    LCD->LCD_REG = regval;
}

//写LCD数据
static void LCD_WR_DATA(u16 data)
{
    LCD->LCD_REG = data;
}

//读LCD数据
static u16 LCD_RD_DATA(void)
{
    return LCD->LCD_RAM;
}

//写寄存器
static void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
    LCD->LCD_REG = LCD_Reg;
    LCD->LCD_RAM = LCD_RegValue;
}

//读寄存器
static u16 LCD_ReadReg(u16 LCD_Reg)
{
    LCD_WR_REG(LCD_Reg);
    delay_us(5);
    return LCD_RD_DATA();
}

//写GRAM
void LCD_WriteRAM_Prepare(void)
{
    LCD->LCD_REG = lcddev.wramcmd;
}
////LCD写GRAM
//static void LCD_WriteRAM(u16 RGB_Code)
//{
//    LCD->LCD_RAM = RGB_Code;
//}

//static u16 LCD_BGR2RGB(u16 c)
//{
//    u16 r, g, b, rgb;
//    b = (c >> 0) & 0x1f;
//    g = (c >> 5) & 0x3f;
//    r = (c >> 11) & 0x1f;
//    rgb = (b << 11) + (g << 5) + (r << 0);
//    return rgb;
//}

void LCD_Scan_Dir(void)
{
    u16 regval = 0;
    u16 dirreg = 0;
    regval |= (1 << 5) | (1 << 4) | (0 << 3);
    dirreg = 0X03;
    regval |= 1 << 12;
    LCD_WriteReg(dirreg, regval);
}

static void LCD_Display_Dir(u8 dir)
{
    if (dir == 0)
    {
        lcddev.dir = 0;
        lcddev.width = 240;
        lcddev.height = 320;
        lcddev.wramcmd = 0x22;
        lcddev.setxcmd = 0x20;
        lcddev.setycmd = 0x21;
    }
    LCD_Scan_Dir(); //默认扫描方向
}

static void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_WriteReg(lcddev.setxcmd, Xpos);
    LCD_WriteReg(lcddev.setycmd, Ypos);
}

static void LCD_Clear(u32 color)
{
    u32 index = 0;
    u32 totalpoint = lcddev.width;
    totalpoint *= lcddev.height;
    LCD_SetCursor(0x00, 0x00);
    LCD_WriteRAM_Prepare(); //开始填充，地址是增长的
    for (index = 0; index < totalpoint; index++)
    {
        LCD->LCD_RAM = color;
    }
}
void show_bmp(u16 * color)
{
    u32 index = 0;
    u32 totalpoint = lcddev.width;
    totalpoint *= lcddev.height;
    LCD_SetCursor(0x00, 0x00);
    LCD_WriteRAM_Prepare(); //开始填充，地址是增长的
    for (index = 0; index < totalpoint; index++)
    {
        LCD->LCD_RAM = *color;
        color++;
    }
}

void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
    u8 hsareg, heareg, vsareg, veareg;
    u16 hsaval, heaval, vsaval, veaval;
    width = sx + width - 1;
    height = sy + height - 1;

    hsaval = sx;
    heaval = width;
    vsaval = sy;
    veaval = height;

    hsareg = 0X50;
    heareg = 0X51;
    vsareg = 0X52;
    veareg = 0X53;
    LCD_WriteReg(hsareg, hsaval);
    LCD_WriteReg(heareg, heaval);
    LCD_WriteReg(vsareg, vsaval);
    LCD_WriteReg(veareg, veaval);
    LCD_SetCursor(sx, sy);
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_FSMC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 |
                       GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                       GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_12;
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);
}

void LCD_Init(void)
{
    GPIO_InitTypeDef LCD_Initure;

    FSMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
    FSMC_NORSRAM_TimingTypeDef FSMC_WriteTim;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    LCD_Initure.Pin = GPIO_PIN_0;
    LCD_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    LCD_Initure.Pull = GPIO_PULLDOWN;
    LCD_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &LCD_Initure);

    TFTSRAM_Handler.Instance = FSMC_NORSRAM_DEVICE;
    TFTSRAM_Handler.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

    TFTSRAM_Handler.Init.NSBank = FSMC_NORSRAM_BANK4;
    TFTSRAM_Handler.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    TFTSRAM_Handler.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    TFTSRAM_Handler.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    TFTSRAM_Handler.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    TFTSRAM_Handler.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    TFTSRAM_Handler.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    TFTSRAM_Handler.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    TFTSRAM_Handler.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    TFTSRAM_Handler.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
    TFTSRAM_Handler.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    TFTSRAM_Handler.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;

    //读时序控
    FSMC_ReadWriteTim.AddressSetupTime = 0x06;
    FSMC_ReadWriteTim.AddressHoldTime = 0;
    FSMC_ReadWriteTim.DataSetupTime = 26;
    FSMC_ReadWriteTim.AccessMode = FSMC_ACCESS_MODE_A;

    //写时序
    FSMC_WriteTim.AddressSetupTime = 3;
    FSMC_WriteTim.BusTurnAroundDuration = 0;
    FSMC_WriteTim.AddressHoldTime = 0;
    FSMC_WriteTim.AccessMode = FSMC_ACCESS_MODE_A;
    FSMC_WriteTim.DataSetupTime = 0x06;
    HAL_SRAM_Init(&TFTSRAM_Handler, &FSMC_ReadWriteTim, &FSMC_WriteTim);

    delay_ms(50);
    LCD_WriteReg(0x0000, 0x0001);
    delay_ms(50);
    lcddev.id = LCD_ReadReg(0x0000);
    printf("LCD id is %X\r\n", lcddev.id);
    if (lcddev.id == 0x9325) //9325
    {
        LCD_WriteReg(0x00E5, 0x78F0);
        LCD_WriteReg(0x0001, 0x0100);
        LCD_WriteReg(0x0002, 0x0700);
        LCD_WriteReg(0x0003, 0x1030);
        LCD_WriteReg(0x0004, 0x0000);
        LCD_WriteReg(0x0008, 0x0202);
        LCD_WriteReg(0x0009, 0x0000);
        LCD_WriteReg(0x000A, 0x0000);
        LCD_WriteReg(0x000C, 0x0000);
        LCD_WriteReg(0x000D, 0x0000);
        LCD_WriteReg(0x000F, 0x0000);
        //power on sequence VGHVGL
        LCD_WriteReg(0x0010, 0x0000);
        LCD_WriteReg(0x0011, 0x0007);
        LCD_WriteReg(0x0012, 0x0000);
        LCD_WriteReg(0x0013, 0x0000);
        LCD_WriteReg(0x0007, 0x0000);
        //vgh
        LCD_WriteReg(0x0010, 0x1690);
        LCD_WriteReg(0x0011, 0x0227);
        //delayms(100);
        //vregiout
        LCD_WriteReg(0x0012, 0x009D); //0x001b
        //delayms(100);
        //vom amplitude
        LCD_WriteReg(0x0013, 0x1900);
        //delayms(100);
        //vom H
        LCD_WriteReg(0x0029, 0x0025);
        LCD_WriteReg(0x002B, 0x000D);
        //gamma
        LCD_WriteReg(0x0030, 0x0007);
        LCD_WriteReg(0x0031, 0x0303);
        LCD_WriteReg(0x0032, 0x0003); // 0006
        LCD_WriteReg(0x0035, 0x0206);
        LCD_WriteReg(0x0036, 0x0008);
        LCD_WriteReg(0x0037, 0x0406);
        LCD_WriteReg(0x0038, 0x0304); //0200
        LCD_WriteReg(0x0039, 0x0007);
        LCD_WriteReg(0x003C, 0x0602); // 0504
        LCD_WriteReg(0x003D, 0x0008);
        //ram
        LCD_WriteReg(0x0050, 0x0000);
        LCD_WriteReg(0x0051, 0x00EF);
        LCD_WriteReg(0x0052, 0x0000);
        LCD_WriteReg(0x0053, 0x013F);
        LCD_WriteReg(0x0060, 0xA700);
        LCD_WriteReg(0x0061, 0x0001);
        LCD_WriteReg(0x006A, 0x0000);
        //
        LCD_WriteReg(0x0080, 0x0000);
        LCD_WriteReg(0x0081, 0x0000);
        LCD_WriteReg(0x0082, 0x0000);
        LCD_WriteReg(0x0083, 0x0000);
        LCD_WriteReg(0x0084, 0x0000);
        LCD_WriteReg(0x0085, 0x0000);
        //
        LCD_WriteReg(0x0090, 0x0010);
        LCD_WriteReg(0x0092, 0x0600);

        LCD_WriteReg(0x0007, 0x0133);
        LCD_WriteReg(0x00, 0x0022); //
    }
    LCD_Display_Dir(0);
    LCD_LED = 0;
    LCD_Clear(WHITE);
}

void LCD_DrawPoint(u16 x, u16 y)
{
    LCD_SetCursor(x, y);
    LCD_WriteRAM_Prepare();
    LCD->LCD_RAM = POINT_COLOR;
}

/**
 * x1:横轴起点
 * y1:纵轴起点
 * x2:横轴终点
 * y2:纵轴终点
 **/

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算横坐标的增量
    delta_y = y2 - y1; //计算纵坐标的增量
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1;
    else if (delta_x == 0)
        incx = 0;
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0;
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x;
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t++)
    {
        LCD_DrawPoint(uRow, uCol);
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}

void LCD_Draw_Circle(u16 x0, u16 y0, u8 r)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1); //判断下个点位置的标志
    while (a <= b)
    {
        LCD_DrawPoint(x0 + a, y0 - b); //5
        LCD_DrawPoint(x0 + b, y0 - a); //0
        LCD_DrawPoint(x0 + b, y0 + a); //4
        LCD_DrawPoint(x0 + a, y0 + b); //6
        LCD_DrawPoint(x0 - a, y0 + b); //1
        LCD_DrawPoint(x0 - b, y0 + a);
        LCD_DrawPoint(x0 - a, y0 - b); //2
        LCD_DrawPoint(x0 - b, y0 - a); //7
        a++;
        //使用Bresenham算法画园
        if (di < 0)
            di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u32 color)
{
    u16 i, j;
    u16 xlen = 0;
    xlen = ex - sx + 1;
    for (i = sy; i <= ey; i++)
    {
        LCD_SetCursor(sx, i);
        LCD_WriteRAM_Prepare();
        for (j = 0; j < xlen; j++)
            LCD->LCD_RAM = color;
    }
}

void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color)
{
    u16 height, width;
    u16 i, j;
    width = ex - sx + 1;
    height = ey - sy + 1;
    for (i = 0; i < height; i++)
    {
        LCD_SetCursor(sx, sy + i);
        LCD_WriteRAM_Prepare();
        for (j = 0; j < width; j++)
            LCD->LCD_RAM = color[i * width + j];
    }
}
