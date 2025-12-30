#include <bcm2835.h>
#include <stdio.h>
#include "st7789.h"

static uint16_t screen[240][240];

static void writeCommand(uint8_t cmd) {
    bcm2835_gpio_clr(TFT_DC);
    bcm2835_spi_transfer(cmd);
}

static void writeData(uint8_t data) {
    bcm2835_gpio_set(TFT_DC);
    bcm2835_spi_transfer(data);
}

void st7789_init(void) {
    bcm2835_spi_begin();
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);
    
    // Hardware reset
    bcm2835_gpio_clr(TFT_RST);
    delay(100);
    bcm2835_gpio_set(TFT_RST);
    delay(100);

    writeCommand(ST7789_SWRESET); // Software reset
    delay(150);

    writeCommand(ST7789_SLPOUT);  // Sleep out
    delay(500);

    writeCommand(ST7789_COLMOD);  // Set color mode
    writeData(0x55);             // 16-bit color
    delay(10);

    writeCommand(ST7789_MADCTL);
    writeData(0x00);             // Normal display

    writeCommand(ST7789_CASET);
    writeData(0x00); writeData(0x00); // XSTART = 0
    writeData(ST7789_TFTWIDTH >> 8); writeData(ST7789_TFTWIDTH & 0xFF); // XEND

    writeCommand(ST7789_RASET);
    writeData(0x00); writeData(0x00); // YSTART = 0
    writeData(ST7789_TFTHEIGHT >> 8); writeData(ST7789_TFTHEIGHT & 0xFF); // YEND

    writeCommand(ST7789_DISPON);  // Display on
    delay(100);
}

// 화면 전체를 특정 색으로 채우기 (버퍼만)
void st7789_clearScreen(uint16_t color) {
    for(int i = 0; i < 240; i++) {
        for(int j = 0; j < 240; j++) {
            screen[i][j] = color;
        }
    }
}

// 사각형 그리기 (버퍼에)
void st7789_drawRect(int x, int y, int w, int h, uint16_t color) {
    for(int i = y; i < y + h && i < 240; i++) {
        if(i < 0) continue;
        for(int j = x; j < x + w && j < 240; j++) {
            if(j >= 0) {
                screen[i][j] = color;
            }
        }
    }
}

// 버퍼를 화면에 렌더링
void st7789_render(void) {
    writeCommand(ST7789_RASET);
    writeData(0); writeData(0);
    writeData(ST7789_TFTHEIGHT >> 8); writeData(ST7789_TFTHEIGHT & 0xFF);
    
    writeCommand(ST7789_CASET);
    writeData(0); writeData(0);
    writeData(ST7789_TFTWIDTH >> 8); writeData(ST7789_TFTWIDTH & 0xFF);
    
    writeCommand(ST7789_RAMWR);
    bcm2835_gpio_set(TFT_DC);
    
    for(int i = 0; i < 240; i++) {
        for(int j = 0; j < 240; j++) {
            bcm2835_spi_transfer(screen[i][j] >> 8);
            bcm2835_spi_transfer(screen[i][j] & 0xFF);
        }
    }
}
