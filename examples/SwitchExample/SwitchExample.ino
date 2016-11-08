#include <Streaming.h>
#include "Switch.h"

const byte toggleSwitchpin = 3;                                     // (right button)
const byte buttonGNDpin    = 4;                                     // (left button)
const byte ButtonVCCpin    = 6;
const byte Button10mspin   = 8;
int i = 0;

Switch buttonGND    = Switch(buttonGNDpin);                         // button to GND, using internal 20K pull-up resistor
Switch toggleSwitch = Switch(toggleSwitchpin);
Switch buttonVCC    = Switch(ButtonVCCpin, INPUT, HIGH);            // button to VCC, 10k pull-down resistor, no internal pull-up resistor, HIGH polarity
Switch button10ms   = Switch(Button10mspin, INPUT_PULLUP, LOW, 1);  // debounceTime: 1ms

void setup() { 
    Serial.begin(9600);
}

void loop() { 
    buttonGND.poll();
    Serial << (buttonGND.on() ? "on" : "off");
    if (buttonGND.pushed()) Serial << "pushed " << ++i << " ";
    if (buttonGND.released()) Serial << "released\n";
    if (toggleSwitch.poll()) Serial << toggleSwitch.on() << endl;
    if (toggleSwitch.longPress()) Serial << "longPress1 ";
    if (toggleSwitch.longPress()) Serial << "longPress2\n";
    if (toggleSwitch.doubleClick()) Serial << "doubleClick1 ";
    if (toggleSwitch.doubleClick()) Serial << "doubleClick2\n";
}