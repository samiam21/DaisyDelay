#include <Arduino.h>
#include <bitset>
#include "DaisyDuino.h"
#include "EffectType.h"
#include "PedalConfig.h"

// Global variables
DaisyHardware hw;
size_t num_channels;

// Effect switching parameters
volatile EffectType currentEffectType = UNSET;
volatile EffectType selectedEffectType = UNSET;
IEffect* currentEffect;

/**
 * Sets the selected effect type based on reading the selector
 */
void ReadSelectedEffect() {
    // Read the state of the hex switch pins
    std::bitset<4> pin1(digitalRead(effectSelectorPin1));
    std::bitset<4> pin2(digitalRead(effectSelectorPin2));
    std::bitset<4> pin3(digitalRead(effectSelectorPin3));
    std::bitset<4> pin4(digitalRead(effectSelectorPin4));    

    // Get the combined hex value and convert it to an int
    std::bitset<4> combined = pin1 | (pin2 << 1) | (pin3 << 2) | (pin4 << 3);
    selectedEffectType = (EffectType)(combined.to_ulong());
}

void setup() {
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
    attachInterrupt(effectSelectorPin1, ReadSelectedEffect, FALLING);
    attachInterrupt(effectSelectorPin1, ReadSelectedEffect, RISING);
    attachInterrupt(effectSelectorPin2, ReadSelectedEffect, FALLING);
    attachInterrupt(effectSelectorPin2, ReadSelectedEffect, RISING);
    attachInterrupt(effectSelectorPin3, ReadSelectedEffect, FALLING);
    attachInterrupt(effectSelectorPin3, ReadSelectedEffect, RISING);
    attachInterrupt(effectSelectorPin4, ReadSelectedEffect, FALLING);
    attachInterrupt(effectSelectorPin4, ReadSelectedEffect, RISING);

    // Read and set the selected effect
    ReadSelectedEffect();
    currentEffect = GetEffectObject(selectedEffectType);

    // Initialize and turn on the control LED
    pinMode(controlLedPin, OUTPUT);
    digitalWrite(controlLedPin, HIGH);
}

void loop() {
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
        DAISY.begin((DaisyDuinoCallback)[](float **in, float **out, size_t size) {return currentEffect->AudioCallback(in, out, size);});

        // Update the current effect type now that we have switched
        currentEffectType = selectedEffectType;
    }

    // Execute the effect loop commands
    currentEffect->Loop();
}