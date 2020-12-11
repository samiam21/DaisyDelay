#include "SimpleBypass.h"

void SimpleBypass::Setup(size_t pNumChannels)
{
    numChannels = pNumChannels;

    // Initialize the knobs
    knob1.Init(effectPotPin1, INPUT, volumeBoost);

    // Initialize the LEDs
    pinMode(effectLedPin1, OUTPUT);
    pinMode(effectLedPin2, OUTPUT);
    pinMode(effectLedPin3, OUTPUT);
}

void SimpleBypass::AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[audioOutChannel][i] = in[audioInChannel][i] * (volumeBoost * 4.0 + 1.0f);
    }
}

void SimpleBypass::Cleanup()
{
    // Turn off the LEDs
    debugPrintln("Turning off LEDs");
    pinMode(effectLedPin1, OUTPUT);
    pinMode(effectLedPin2, OUTPUT);
    pinMode(effectLedPin3, OUTPUT);
    digitalWrite(effectLedPin1, LOW);
    digitalWrite(effectLedPin2, LOW);
    digitalWrite(effectLedPin3, LOW);
}

void SimpleBypass::Loop()
{
    // Knob 1 controls the volume boost
    if (knob1.SetNewValue(volumeBoost))
    {
        debugPrint("Updated the volume boost level to: ");
        debugPrintln(volumeBoost);
    }
}

String SimpleBypass::GetEffectName()
{
    return "Simple Bypass Test";
}