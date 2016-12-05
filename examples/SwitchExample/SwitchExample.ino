#include <Streaming.h>
#include "Switch.h"

const byte toggleGndPin = 3;
const byte toggleVccPin = 5;
const byte buttonGndPin = 7;                                     
const byte buttonVccPin = 9;

byte toggleGndLastState;
byte toggleVccLastState;
byte buttonGndPressCounter = 0;
byte buttonVccPressCounter = 0;

Switch toggleGnd(toggleGndPin);
Switch toggleVcc(toggleVccPin, INPUT, POLARITY_HIGH);
Switch buttonGnd(buttonGndPin);
Switch buttonVcc(buttonVccPin, INPUT, POLARITY_HIGH);            // button to VCC, 10k pull-down resistor, no internal pull-up resistor, HIGH polarity

void setup() {
    Serial.begin(9600);
    toggleGnd.poll();
    toggleGndLastState = toggleGnd.state();
    toggleVcc.poll();
    toggleVccLastState = toggleVcc.state();
}

void loop() {
    toggleGnd.poll();
    if (toggleGnd.state() != toggleGndLastState) {
        Serial << "toggleGnd " << (toggleGnd.state() ? "on\n" : "off\n");
        toggleGndLastState = toggleGnd.state();
    }
    toggleVcc.poll();
    if (toggleVcc.state() != toggleVccLastState) {
        Serial << "toggleVcc " << (toggleVcc.state() ? "on\n" : "off\n");
        toggleVccLastState = toggleVcc.state();
    }
    buttonGnd.poll();
    if (buttonGnd.pushed())      Serial << "buttonGnd pushed (" << buttonGndPressCounter++ << ")\n";
    if (buttonGnd.released())    Serial << "buttonGnd released\n";
    if (buttonGnd.longPress())   Serial << "buttonGnd longPress\n";
    if (buttonGnd.doubleClick()) Serial << "buttonGnd doubleClick\n";  
    buttonVcc.poll();
    if (buttonVcc.pushed())      Serial << "buttonVcc pushed (" << buttonVccPressCounter++ << ")\n";
    if (buttonVcc.released())    Serial << "buttonVcc released\n";
    if (buttonVcc.longPress())   Serial << "buttonVcc longPress\n";
    if (buttonVcc.doubleClick()) Serial << "buttonVcc doubleClick\n";
}

//<< endl