// only one of the following defines can be enabled (set to one)

#include "stdio.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#include "ST7735.h"
#include "SPIScreenIO.h"

#include "PiPico.h"
#include "CodalDmesg.h"
using namespace codal;

#define APP_ID  100
#define APP_EVT_ON 100
#define APP_EVT_OFF 101

PiPico pico;
// test tft
ScreenIO *io;
ST7735 *lcd;
uint8_t *screenBuf;

#define DISPLAY_CFG0 0x80
#define DISPLAY_CFG1 0x603
#define DISPLAY_CFG2 0x16

static uint32_t palette[16] = {
    // alpha, r, g, b
    0xff000000,
    0xffffffff,
    0xffff2121,
    0xffff93c4,
    0xffff8135,
    0xfffff609,
    0xff249ca3,
    0xff78dc52,
    0xff003fad,
    0xff87f2ff,
    0xff8e2ec4,
    0xffa4839f,
    0xff5c406c,
    0xffe5cdc4,
    0xff91463d,
    0xff000000
};

int main() {
    pico.init();
    printf("pico init\r\n");
    io = new SPIScreenIO(pico.spi);
    // dc: 21, rst: 25, cs: 24
    lcd = new ST7735(*io, pico.io.GP24, pico.io.GP21);
    // cs
    pico.io.GP24.setDigitalValue(1);
    // reset lcd
    pico.io.GP25.setDigitalValue(0);
    fiber_sleep(50);
    pico.io.GP25.setDigitalValue(1);

    // tft init
    // pico.spi.setFrequency(15 * 1000000);
    pico.spi.setMode(0);
    lcd->init();
    lcd->configure(DISPLAY_CFG0 & 0xff, DISPLAY_CFG1);
    
    uint8_t offX = (DISPLAY_CFG0 >> 8) & 0xff;
    uint8_t offY = (DISPLAY_CFG0 >> 16) & 0xff;
    uint16_t width = 160;
    uint16_t height = 128;
    int sz = width * height;
    screenBuf = (uint8_t *)malloc(sz / 2 + 20);
    memset(screenBuf, 0, sizeof(screenBuf));

    lcd->setAddrWindow(offX, offY, width, height);
    
    // lcd->startRAMWR();
    // uint16_t tmp = 0xFFFF;
    // for (int i=0;i<128*160;i++){
    //     io->send((uint8_t*)&tmp, 2);
    // }
    // lcd->endCS();
    
    lcd->sendIndexedImage(screenBuf, width, height, palette);

    while(1) {
        DMESG("idle");
        pico.sleep(3000);
    }
}
