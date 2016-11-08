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

                        _______________________      _false
                       |                       |   || |
 input                 |                       || ||| |
                  _____|                       ||_||| ||_______________

 poll                   ^                        ^   ^          ^
_switchedTime           ^                        ^
                         <---------100ms--------><--->
_debounceDelay           <----50ms---->          <----50ms---->
_switched               1                        1   0          0
 newlevel               1                        0   1          0
                         ________________________
_level             _____|                        |_____________________

.......................................................................
                            _______________________             _______
                           |                       |           |
 input                     |                       |           |
                   ________|                       |___________|

_longPressDelay             <----------->

_doubleClickDelay           <-------------------------------------->
                                          _________
_longPressLatch    ______________________|         |_________________
                                          _
_longPress         ______________________| |__________________________
                                                                _
_doubleClick       ____________________________________________| |____

*/

#include "Switch.h"

Switch::Switch(byte pin, byte PinMode, bool polarity, unsigned int debounceDelay, unsigned int longPressDelay, unsigned int doubleClickDelay): _pin(pin), _polarity(polarity), _debounceDelay(debounceDelay), _longPressDelay(longPressDelay), _doubleClickDelay(doubleClickDelay) {
    pinMode(_pin, PinMode);
    _switchedTime = millis();
    _switched = false;
    _longPress = false;
    _longPressLatch = false;
    _doubleClick = false;
    _level = digitalRead(_pin);
}

bool Switch::poll() {
    _longPress = _doubleClick = false;
    bool newlevel = digitalRead(_pin);
    if (!_longPressLatch) {
        _longPress = on() && ((long)(millis() - _pushedTime) > _longPressDelay);   // true just one time between polls
        _longPressLatch = _longPress;                                              // resets at next switch
    }
    if ((newlevel != _level) & (millis() - _switchedTime >= _debounceDelay)) {
        _switchedTime = millis();
        _level = newlevel;
        _switched = true;
        _longPressLatch = false;
        if (pushed()) {
            _doubleClick = (long)(millis() - _pushedTime) < _doubleClickDelay;
            _pushedTime = millis();
        }
        return _switched;
    }
    return _switched = false;
}

bool Switch::on() {
    return !(_level^_polarity);
}

bool Switch::pushed() {
    return _switched && !(_level^_polarity);
}

bool Switch::released() {
    return _switched && (_level^_polarity);
}

bool Switch::longPress() {
    return _longPress;
}

bool Switch::doubleClick() {
    return _doubleClick;
}
