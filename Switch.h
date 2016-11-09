/*
Switch.h
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

..........................................DEGLITCHING..............................
                                           
                        ________________   _
               on      |                | | |    _                        
                       |                | | |   | |                                      
                       |                |_| |___| |__                                                           
 analog        off_____|_____________________________|____________________________  
                   
                        ________________   _     _
 input            _____|                |_| |___| |_______________________________               
            
 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^   
 
 equal           0 1 1 0 1 1 1 1 1 1 1 1 0 0 0 1 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
 
 deglitchPeriod          <--------><--   <--     <-  <--------><--------><--------
                                    ___________________________
 deglitched       _________________|                           |__________________
 
 deglitchTime            ^         ^     ^       ^   ^         ^        ^
 
 ..........................................DEBOUNCING.............................
 
 debouncePeriod                    <-------------------------------->    
                                    _________________________________
 debounced        _________________|                                 |____________  
                                    _                                 _
 _switched        _________________| |_______________________________| |__________       
                                                     
 switchedTime                      ^                                 ^ 

........................................DOUBLE CLICK..............................
                                         
                           __________         ______                           
 debounced        ________|          |_______|      |_____________________________  
 
 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^          
                           _                  _
 pushed          _________| |________________| |__________________________________       
                                                     
 pushedTime               ^                  ^ 
 
 doubleClickPeriod         <------------------------------------->                     
                                              _
 _doubleClick     ___________________________| |__________________________________
 
                             
........................................LONG PRESS................................
                                          
                           ___________________________                                     
 debounced        ________|                           |___________________________        
 
 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^             
         
 longPressPeriod            <--------------->         
                            _                           _
 _switched        _________| |_________________________| |________________________       
                                              __________
 longPressDisable ___________________________|          |_________________________                                  
                                              _
 _longPress       ___________________________| |__________________________________        

*/

#ifndef SWITCH_H
#define SWITCH_H

#if !defined(ARDUINO_ARCH_AVR)
#error “The Switch library only supports AVR processors.”
#endif

#include <Arduino.h>

namespace SwitchLibrary {

    const byte         PINMODE_DEFAULT     =  INPUT_PULLUP;
    const bool         POLARITY_DEFAULT    =  LOW;
    const unsigned int DEGLITCH_PERIOD     =   10;
    const unsigned int DEBOUNCE_PERIOD     =   50;
    const unsigned int LONG_PRESS_PERIOD   =  300;
    const unsigned int DOUBLE_CLICK_PERIOD =  250;

    class Switch {
        public:
            Switch(byte pin,
                   byte PinMode                   = PINMODE_DEFAULT,
                   bool polarity                  = POLARITY_DEFAULT,
                   unsigned int deglitchPeriod    = DEGLITCH_PERIOD,
                   unsigned int debouncePeriod    = DEBOUNCE_PERIOD,
                   unsigned int longPressPeriod   = LONG_PRESS_PERIOD,
                   unsigned int doubleClickPeriod = DOUBLE_CLICK_PERIOD);
//            ~Switch();
            bool poll();                                  // 0 = switched off / 1 = switched on
            bool switched();                              // refreshed by poll()
            bool on();                                    // refreshed by poll()
            bool pushed();                                // refreshed by poll()
            bool released();                              // refreshed by poll()
            bool longPress();                             // refreshed by poll()
            bool doubleClick();                           // refreshed by poll()
        private:
            bool process();
            void inline deglitch();
            void inline debounce();
            void inline calcLongPress();
            void inline calcDoubleClick();
            const byte  _pin;
            const bool  _polarity;
            const unsigned int _deglitchPeriod, _debouncePeriod, _longPressPeriod, _doubleClickPeriod;
            bool  _input, _lastInput, _equal, _deglitched, _debounced, _switched, _longPress, _longPressDisable, _doubleClick;
            unsigned long _deglitchTime, _switchedTime, _pushedTime, _ms;
    };
}

using namespace SwitchLibrary;

#endif
