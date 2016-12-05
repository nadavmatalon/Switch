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

#if 1
__asm volatile ("nop");
#endif

#ifndef SWITCH_H
#define SWITCH_H

#if !defined(ARDUINO_ARCH_AVR)
#error “The Switch library only supports AVR processors.”
#endif

#include <Arduino.h>

namespace SwitchLibrary {

    typedef enum:byte {
        OFF = 0,
        ON  = 1
    } state_t;
    
    typedef enum:byte {
        POLARITY_LOW  = 0,
        POLARITY_HIGH = 1
    } polarity_t;
    
    const byte         DEFAULT_PIN_MODE     =  INPUT_PULLUP;
    const byte         DEFAULT_POLARITY     =  POLARITY_LOW;
    const byte         DEFAULT_DEGLITCH     =   10;
    const byte         DEFAULT_DEBOUNCE     =   50;
    const unsigned int DEFAULT_LONG_PRESS   =  300;
    const unsigned int DEFAULT_DOUBLE_CLICK =  250;

    class Switch {
        public:
            Switch(
                byte pin,
                byte PinMode                   = DEFAULT_PIN_MODE,
                polarity_t polarity            = DEFAULT_POLARITY,
                unsigned int deglitchPeriod    = DEFAULT_DEGLITCH,
                unsigned int debouncePeriod    = DEFAULT_DEBOUNCE,
                unsigned int longPressPeriod   = DEFAULT_LONG_PRESS,
                unsigned int doubleClickPeriod = DEFAULT_DOUBLE_CLICK
            );
            ~Switch();
            void poll();                                  // 0 = switched off / 1 = switched on
            byte state();                                 // refreshed by poll()
            byte pushed();                                // refreshed by poll()
            byte released();                              // refreshed by poll()
            byte longPress();                             // refreshed by poll()
            byte doubleClick();                           // refreshed by poll()
        private:
            void inline deglitch();
            void inline debounce();
            void inline calcLongPress();
            void inline calcDoubleClick();
            byte on();
            const byte  _pin, _polarity;
            const unsigned int _deglitchPeriod, _debouncePeriod, _longPressPeriod, _doubleClickPeriod;
            byte  _state, _input, _lastInput, _equal, _deglitched, _debounced, _longPress, _longPressDisable, _doubleClick;
            unsigned long  _ms, _deglitchTime, _switchedTime, _pushedTime;
    };
}

using namespace SwitchLibrary;

#endif
