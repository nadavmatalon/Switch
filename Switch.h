/*
Switch

Copyright (C) 2012  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .
 
Version 20-4-2013:          _debounceDelay=50
Version 22-5-2013:          Added longPress, doubleClick
Version 2.3.0 (8-11-2016):  refactoring & stylistic modifications

*/

#ifndef SWITCH_H
#define SWITCH_H

#if !defined(ARDUINO_ARCH_AVR)
#error “The Switch library only supports AVR processors.”
#endif

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace SwitchDebouncer {
    
    class Switch {
        public:
            Switch(byte pin, byte PinMode = INPUT_PULLUP, bool polarity = LOW, unsigned int debounceDelay = 50, unsigned int longPressDelay = 400, unsigned int doubleClickDelay = 250);
            bool poll();                                  // 0 = switched off / 1 = switched on
            bool on();
            bool pushed();                                // refreshed by poll()
            bool released();                              // refreshed by poll()
            bool longPress();                             // refreshed by poll()
            bool doubleClick();                           // refreshed by poll()
        private:
            const byte _pin;
            const bool _polarity;
            const unsigned int  _debounceDelay, _longPressDelay, _doubleClickDelay;
            unsigned long _switchedTime, _pushedTime;
            bool _level, _switched, _longPress, _longPressLatch, _doubleClick;
    };
}

using namespace SwitchDebouncer;

#endif
