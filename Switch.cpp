/*
Switch.cpp
Copyright (C) 2012  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .

Version 20-4-2013:          _debouncePeriod = 50
Version 22-5-2013           added _longPress, _doubleClick
Version 1-12-2015           added _process(_input)
Version 15-1-2016           added deglitching
Version 2.3.0 (8-11-2016):  refactoring & stylistic modifications

*/

#if 1
__asm volatile ("nop");
#endif

#include "Switch.h"

Switch::Switch(byte         pin,
               byte         PinMode,
               polarity_t   polarity,
               unsigned int deglitchPeriod,
               unsigned int debouncePeriod,
               unsigned int longPressPeriod,
               unsigned int doubleClickPeriod) :
               _pin(pin),
               _polarity(polarity),
               _deglitchPeriod(deglitchPeriod),
               _debouncePeriod(debouncePeriod),
               _longPressPeriod(longPressPeriod),
               _doubleClickPeriod(doubleClickPeriod)
               {
                   pinMode(_pin, PinMode);
                   _switchedTime = millis();
                   _deglitchTime = 0;
                   _pushedTime = 0;
                   _debounced = digitalRead(_pin);
                   poll();
               }

Switch::~Switch() {};

void Switch::poll() {
    _input = digitalRead(_pin);
    deglitch();
    debounce();
    calcLongPress();
    calcDoubleClick();
}

byte Switch::state() {
    return _state;
}

byte Switch::pushed() {
    return (_state && !(_debounced ^ _polarity));
}

byte Switch::released() {
    return (_state && (_debounced ^ _polarity));
}

byte Switch::longPress() {
    return _longPress;
}

byte Switch::doubleClick() {
    return _doubleClick;
}

void inline Switch::deglitch() {
    _ms = millis();
    if (_input == _lastInput) _equal = 1;
    else {
        _equal = 0;
        _deglitchTime = _ms;
    }
    if (_equal & ((_ms - _deglitchTime) > _deglitchPeriod)) {                  // max 50ms, disable deglitch: 0ms
        _deglitchTime = _ms;
        _deglitched = _input;
    }
    _lastInput = _input;
}

void inline Switch::debounce() {
    _ms = millis();
    _state = OFF;
    if ((_deglitched != _debounced) & ((_ms - _switchedTime) >= _debouncePeriod)) {
        _switchedTime = _ms;
        _debounced = _deglitched;
        _state = ON;
        _longPressDisable = OFF;
    }
}

void inline Switch::calcLongPress() {
    _longPress = OFF;
    if (!_longPressDisable) {
        _longPress = (on() && ((_ms - _pushedTime) > _longPressPeriod));       // true just one time between polls
        _longPressDisable = _longPress;                                        // will be reset at next switch
    }
}

void inline Switch::calcDoubleClick() {
    _doubleClick = OFF;
    if (pushed()) {
        _doubleClick = ((_ms - _pushedTime) < _doubleClickPeriod);             // pushedTime of previous push
        _pushedTime = _ms;
    }
}

byte Switch::on() {
    return !(_debounced ^ _polarity);
}
