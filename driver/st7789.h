#pragma once

#include <bcm2835.h>
#include <stdio.h>

// Pin definitions
#define TFT_CS      8
#define TFT_DC      25
#define TFT_RST     24

#define ST7789_TFTWIDTH  240
#define ST7789_TFTHEIGHT 240

// ST7789 commands
#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_SLPOUT  0x11
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_MADCTL  0x36
#define ST7789_COLMOD  0x3A

// 색상 정의
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F

void st7789_init(void);
void st7789_clearScreen(uint16_t color);
void st7789_drawRect(int x, int y, int w, int h, uint16_t color);
void st7789_render(void);
