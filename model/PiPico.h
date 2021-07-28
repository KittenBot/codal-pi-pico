/*
The MIT License (MIT)

Copyright (c) 2017 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef PI_PICO_H
#define PI_PICO_H


#include "CodalConfig.h"
#include "CodalHeapAllocator.h"
#include "CodalDevice.h"
#include "ErrorNo.h"
#include "CodalCompat.h"
#include "CodalComponent.h"
#include "ManagedType.h"
#include "ManagedString.h"
#include "Event.h"
#include "NotifyEvents.h"

#include "Button.h"
#include "Timer.h"
#include "RP2040LowLevelTimer.h"
#include "RP2040Spi.h"
#include "RP2040I2C.h"

#include "PiPicoIO.h"

#include "CodalFiber.h"
#include "MessageBus.h"

namespace codal
{
    /**
      * Class definition for a PiPico device.
      *
      * Represents the device as a whole, and includes member variables that represent various device drivers
      * used to control aspects of the device.
      */
    class PiPico : public CodalDevice, public CodalComponent
    {
        private:



        /**
          * A listener to perform actions as a result of Message Bus reflection.
          *
          * In some cases we want to perform lazy instantiation of components, such as
          * the compass and the accelerometer, where we only want to add them to the idle
          * fiber when someone has the intention of using these components.
          */
        void                        onListenerRegisteredEvent(Event evt);
        uint8_t                     status;

        public:
        RP2040LowLevelTimer         sysTimer;
        Timer                       timer;
        MessageBus                  messageBus;
        PiPicoIO                    io;
        RP2040SPI                   spi;
        RP2040I2C                   i2c;

        /**
          * Constructor.
          *
          * Create a representation of a Genuino Zero device, which includes member variables
          * that represent various device drivers used to control aspects of the board.
          */
        PiPico();

        void idleCallback();
    };

    /**
      * A listener to perform actions as a result of Message Bus reflection.
      *
      * In some cases we want to perform lazy instantiation of components, such as
      * the compass and the accelerometer, where we only want to add them to the idle
      * fiber when someone has the intention of using these components.
      */
    void onListenerRegisteredEvent(Event evt);
}

using namespace codal;

#endif