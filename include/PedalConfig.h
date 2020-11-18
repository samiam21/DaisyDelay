#ifndef PEDAL_CONFIG_H
#define PEDAL_CONFIG_H

#include "DaisyDuino.h"

#define DEBUG 1

#define initDebugPrint() {if(DEBUG) Serial.begin(9600);}
#define debugPrint(msg) {if(DEBUG) Serial.println(msg);}

const int controlLedPin = LED_BUILTIN; // Built in LED is LED_BUILTIN

// Pin Definitions - Selector
const int effectSelectorPin1 = 29;
const int effectSelectorPin2 = 28;
const int effectSelectorPin3 = 27;
const int effectSelectorPin4 = 26;

// Pin Definitions - SPST
const int effectSPSTPin1 = 6;
const int effectSPSTPin2 = 7;
const int effectSPSTPin3 = 8;
const int effectSPSTPin4 = 5;

// Pin Definitions - SPDT
const int effectSPDT1Pin1 = 12;
const int effectSPDT1Pin2 = 13;
const int effectSPDT2Pin1 = 10;
const int effectSPDT2Pin2 = 11;

// Pin Definitions - POT
const int effectPotPin1   = 23;
const int effectPotPin2   = 22;
const int effectPotPin3   = 21;
const int effectPotPin4   = 20;

// Pin Definitions - LED
const int effectLedPin1    = 16;
const int effectLedPin2    = 17;
const int effectLedPin3    = 15;
const int effectLedPin4    = 18;

#endif