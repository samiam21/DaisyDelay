#include <bitset>
#include "DaisyDuino.h"
#include "src/EffectType.h"
#include "PedalConfig.h"

// Global variables
DaisyHardware hw;
size_t num_channels;

// Effect switching parameters
EffectType currentEffectType = UNSET;
IEffect* currentEffect;
IEffect* newEffect;

void setup() 
{
    // Initialize the serial debug output
    initDebugPrint();
    debugPrint("Starting DaisyPedal...");

    // Initialize Daisy at 96kHz
    hw = DAISY.init(DAISY_SEED, AUDIO_SR_96K);
    num_channels = hw.num_channels;

    // Initialize the hex switch pins
    pinMode(effectSelectorPin1, INPUT_PULLDOWN);
    pinMode(effectSelectorPin2, INPUT_PULLDOWN);
    pinMode(effectSelectorPin3, INPUT_PULLDOWN);
    pinMode(effectSelectorPin4, INPUT_PULLDOWN);

    // Initialize the control LED
    pinMode(controlLedPin, OUTPUT);
}

void loop() 
{
    // Read the state of the hex switch pins
    std::bitset<4> pin1(digitalRead(effectSelectorPin1));
    std::bitset<4> pin2(digitalRead(effectSelectorPin2));
    std::bitset<4> pin3(digitalRead(effectSelectorPin3));
    std::bitset<4> pin4(digitalRead(effectSelectorPin4));    

    // Get the combined hex value and convert it to an int
    std::bitset<4> combined = pin1 | (pin2 << 1) | (pin3 << 2) | (pin4 << 3);
    EffectType newEffectType = (EffectType)(combined.to_ulong());

    // Get the current effect pointer
    currentEffect = GetEffectObject(currentEffectType);

    // Check if the state is new and switch to the new state
    if (currentEffectType != newEffectType)
    {
        // A new effect has been chosen, stop the old effect
        currentEffect->Cleanup();

        // Start the new effect
        newEffect = GetEffectObject(newEffectType);
        debugPrint("Switching to: " + newEffect->GetEffectName());
        newEffect->Setup(num_channels);
        DAISY.begin((DaisyDuinoCallback)[](float **in, float **out, size_t size) {return newEffect->AudioCallback(in, out, size);});

        // Update the current effect
        currentEffectType = newEffectType;
    }

    // Execute the effect loop commands
    currentEffect->Loop();
}
