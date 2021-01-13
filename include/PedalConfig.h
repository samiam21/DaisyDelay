#ifndef PEDAL_CONFIG_H
#define PEDAL_CONFIG_H

#include "DaisyDuino.h"

#define DEBUG 1

#define BLOCKSIZE 1
#define DAISY_SAMPLE_RATE AUDIO_SR_96K

// NOTE: If you bypass the selector, make sure the selectedEffectType in main.cpp is set to the desired effect
#define BYPASS_SELECTOR // Bypasses the effect selector

#define initDebugPrint()        \
    {                           \
        if (DEBUG)              \
            Serial.begin(9600); \
    }
#define debugPrintln(msg)        \
    {                            \
        if (DEBUG)               \
            Serial.println(msg); \
    }
#define debugPrint(msg)        \
    {                          \
        if (DEBUG)             \
            Serial.print(msg); \
    }
#define debugPrintlnF(msg, decimalPlaces)       \
    {                                           \
        if (DEBUG)                              \
            Serial.println(msg, decimalPlaces); \
    }
#define debugPrintF(msg, decimalPlaces)       \
    {                                         \
        if (DEBUG)                            \
            Serial.print(msg, decimalPlaces); \
    }

#define PI_VAL 3.14159265

const int controlLedPin = LED_BUILTIN; // Built in LED is LED_BUILTIN

// Audio channels
const int audioInChannel = 0;
const int audioOutChannel = 0;
const int audioInStereoChannel = -1;
const int audioOutStereoChannel = -1;

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
const int effectPotPin1 = 23;
const int effectPotPin2 = 22;
const int effectPotPin3 = 21;
const int effectPotPin4 = 20;

// Pin Definitions - LED
const int effectLedPin1 = 16;
const int effectLedPin2 = 17;
const int effectLedPin3 = 15;
const int effectLedPin4 = 18;

#endif