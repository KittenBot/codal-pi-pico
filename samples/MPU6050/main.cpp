#include "stdio.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#include "MPU6050.h"
#include "CoordinateSystem.h"

#include "PiPico.h"
#include "CodalDmesg.h"
using namespace codal;

PiPico pico;
MPU6050 * imu;


int main() {
    pico.init();

    int space = 0x33;
    auto coord = CoordinateSpace((CoordinateSystem)(space & 0xf), !!(space & 0x100), (space >> 4) & 0xf);
    imu = new MPU6050(pico.i2c, pico.io.GP11, coord);
    imu->requestUpdate();
    

    while(1) {
        printf("ACC: [X:%d Y:%d Z:%d]\r\n", imu->getX(), imu->getY(), imu->getZ());
        pico.sleep(200);
    }
}
