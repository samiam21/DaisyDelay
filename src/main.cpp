#include <Arduino.h>
#include "DaisyDuino.h"
#include "EffectType.h"
#include "PedalConfig.h"

// Global variables
DaisyHardware hw;
size_t num_channels;

// Effect switching parameters
volatile EffectType currentEffectType = UNSET;
volatile EffectType selectedEffectType = UNSET;
IEffect *currentEffect;

/**
 * Sets the selected effect type based on reading the selector
 */
void ReadSelectedEffect()
{
    // Read the state of the encoder pins
    uint32_t pin1 = (uint32_t)digitalRead(effectSelectorPin1);
    uint32_t pin2 = (uint32_t)digitalRead(effectSelectorPin2);
    uint32_t pin3 = (uint32_t)digitalRead(effectSelectorPin3);
    uint32_t pin4 = (uint32_t)digitalRead(effectSelectorPin4);

    // Get the combined value and set the effect type
    uint32_t combined = pin4 | (pin3 << 1) | (pin2 << 2) | (pin1 << 3);
    selectedEffectType = (EffectType)(combined);
}

void setup()
{
    // // Initialize the serial debug output
    initDebugPrint();
    debugPrint("Starting DaisyPedal...");

    // Initialize Daisy at 96kHz
    hw = DAISY.init(DAISY_SEED, AUDIO_SR_96K);
    num_channels = hw.num_channels;

    // Initialize the encoder pins
    pinMode(effectSelectorPin1, INPUT_PULLDOWN);
    pinMode(effectSelectorPin2, INPUT_PULLDOWN);
    pinMode(effectSelectorPin3, INPUT_PULLDOWN);
    pinMode(effectSelectorPin4, INPUT_PULLDOWN);

    // Attach interrupts to each of the encoder pins
    //  the encoder is grey coded so only one of these will happen at a time
    attachInterrupt(effectSelectorPin1, ReadSelectedEffect, CHANGE);
    attachInterrupt(effectSelectorPin2, ReadSelectedEffect, CHANGE);
    attachInterrupt(effectSelectorPin3, ReadSelectedEffect, CHANGE);
    attachInterrupt(effectSelectorPin4, ReadSelectedEffect, CHANGE);

    // Read and set the selected effect
    ReadSelectedEffect();
    currentEffect = GetEffectObject(selectedEffectType);

    // Initialize and turn on the control LED
    pinMode(controlLedPin, OUTPUT);
    digitalWrite(controlLedPin, HIGH);
}

void loop()
{
    // Check if we have a new effect type and switch to the new state
    if (currentEffectType != selectedEffectType)
    {
        // Clean up and stop the old effect
        currentEffect->Cleanup();

        // Get the new effect object
        currentEffect = GetEffectObject(selectedEffectType);

        // Start the new effect
        debugPrint("Switching to: " + currentEffect->GetEffectName());
        currentEffect->Setup(num_channels);
        DAISY.begin((DaisyDuinoCallback)[](float **in, float **out, size_t size) { return currentEffect->AudioCallback(in, out, size); });

        // Update the current effect type now that we have switched
        currentEffectType = selectedEffectType;
    }

    // Execute the effect loop commands
    currentEffect->Loop();
}