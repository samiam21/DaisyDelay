#include <Arduino.h>
#include "DaisyDuino.h"
#include "EffectType.h"
#include "PedalConfig.h"

// Global variables
DaisyHardware hw;
size_t num_channels;

// Effect switching parameters
volatile EffectType currentEffectType = SINGLEECHO;
volatile EffectType selectedEffectType = SINGLEECHO;
IEffect *currentEffect;

/**
 * Sets the selected effect type based on reading the selector
 */
bool ReadSelectedEffect()
{
    // Read the state of the encoder pins
    uint32_t pin1 = (uint32_t)digitalRead(effectSelectorPin1);
    uint32_t pin2 = (uint32_t)digitalRead(effectSelectorPin2);
    uint32_t pin3 = (uint32_t)digitalRead(effectSelectorPin3);
    uint32_t pin4 = (uint32_t)digitalRead(effectSelectorPin4);

    // Get the combined value and set the effect type
    uint32_t combined = pin4 | (pin3 << 1) | (pin2 << 2) | (pin1 << 3);

    if ((EffectType)combined != selectedEffectType)
    {
        selectedEffectType = (EffectType)(combined);
        return true;
    }
    else
    {
        return false;
    }
}

void setup()
{
    // // Initialize the serial debug output
    initDebugPrint();
    debugPrintln("Starting DaisyPedal...");

    // Initialize Daisy at 96kHz
    hw = DAISY.init(DAISY_SEED, AUDIO_SR_96K);
    num_channels = hw.num_channels;

#ifndef BYPASS_SELECTOR
    // Initialize the encoder pins
    pinMode(effectSelectorPin1, INPUT);
    pinMode(effectSelectorPin2, INPUT);
    pinMode(effectSelectorPin3, INPUT);
    pinMode(effectSelectorPin4, INPUT);

    // Read the selected effect
    ReadSelectedEffect();
#endif

    // Set the current effect
    currentEffect = GetEffectObject(selectedEffectType);

    // Start the effect
    debugPrintln("Starting: " + currentEffect->GetEffectName());
    currentEffect->Setup(num_channels);
    DAISY.begin((DaisyDuinoCallback)[](float **in, float **out, size_t size) { return currentEffect->AudioCallback(in, out, size); });

    // Initialize and turn on the control LED
    pinMode(controlLedPin, OUTPUT);
    digitalWrite(controlLedPin, HIGH);
}

void loop()
{
#ifndef BYPASS_SELECTOR
    // Check if we have a new effect type and switch to the new state
    if (ReadSelectedEffect())
    {
        // Clean up and stop the old effect
        currentEffect->Cleanup();

        // Get the new effect object
        currentEffect = GetEffectObject(selectedEffectType);

        // Start the new effect
        debugPrintln("Switching to: " + currentEffect->GetEffectName());
        currentEffect->Setup(num_channels);
        DAISY.begin((DaisyDuinoCallback)[](float **in, float **out, size_t size) { return currentEffect->AudioCallback(in, out, size); });
    }
#endif

    // Execute the effect loop commands
    currentEffect->Loop();
}