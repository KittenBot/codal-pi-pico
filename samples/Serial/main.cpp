#include "stdio.h"

#include "Synthesizer.h"
#include "Mixer.h"
#include "RP2040PWM.h"

#include "PiPico.h"
#include "CodalDmesg.h"
#include "ZSingleWireSerial.h"

#include "hardware/clocks.h"

using namespace codal;

PiPico pico;
ZSingleWireSerial * sws;
uint8_t rxbuff[256];

char * txt = "hello world";

void print_buf(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
        if (i % 16 == 15)
            printf("\n");
        else
            printf(" ");
    }
}

void sws_done(uint16_t errCode) {


}

int main() {
    float abc = 123.0;
    printf("rom %d %02f\r\n", rp2040_rom_version(), abc);

    pico.init();
    sws = new ZSingleWireSerial(pico.io.GP9);
#if 0
    while(1) {
        sws->sendDMA((uint8_t *)txt, strlen(txt));
        pico.sleep(1);
        sws->abortDMA();
        sws->p.setDigitalValue(0);
        pico.sleep(2);
        sws->p.setDigitalValue(1);
        pico.sleep(2000);
    }
#else
    memset(rxbuff, 0, 256);
    sws->receiveDMA(rxbuff, 256);
    sws->setIRQ(sws_done);
    while(1){
        pico.sleep(2000);
        print_buf(rxbuff, 256);
    }
#endif
}
