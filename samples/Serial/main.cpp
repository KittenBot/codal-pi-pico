#include "stdio.h"

#include "Synthesizer.h"
#include "Mixer.h"
#include "RP2040PWM.h"

#include "PiPico.h"
#include "CodalDmesg.h"
#include "ZSingleWireSerial.h"

#include "hardware/clocks.h"

using namespace codal;

#define APP_ID  100
#define APP_TX_DONE 101
#define APP_RX_DONE 102

PiPico pico;
ZSingleWireSerial * sws;

uint8_t txBuff[256];
uint8_t rxBuff[256];

void print_buf(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
        if (i % 16 == 15)
            printf("\n");
        else
            printf(" ");
    }
}

void sws_done(uint16_t code) {
    switch (code)
    {
    case SWS_EVT_DATA_SENT:
        Event(APP_ID, APP_TX_DONE);
        break;
    case SWS_EVT_DATA_RECEIVED:
        Event(APP_ID, APP_RX_DONE);
        break;
    }

}

int main() {
    float abc = 123.0;
    printf("rom %d %02f\r\n", rp2040_rom_version(), abc);

    for (int i=0;i<256;i++){
        txBuff[i] = i;
    }

    pico.init();
    sws = new ZSingleWireSerial(pico.io.GP9);
    
    sws->setIRQ(sws_done);
    while (1)
    {
        sws->sendDMA(txBuff, 256);
        fiber_wait_for_event(APP_ID, APP_TX_DONE);
        sws->abortDMA();
        printf("ping >>>\r\n");
        pico.sleep(3000);
        
        memset(rxBuff,0,256);
        sws->receiveDMA(rxBuff, 256);
        fiber_wait_for_event(APP_ID, APP_RX_DONE);
        printf("<<< pong\r\n");
        print_buf(rxBuff, 256);
    }
}
