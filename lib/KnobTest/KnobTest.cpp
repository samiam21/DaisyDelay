#include "KnobTest.h"

void KnobTest::Setup(size_t pNumChannels)
{
    numChannels = pNumChannels;

    // Initialize the knobs
    knob1.Init(effectPotPin1, INPUT, led1Value);
    knob2.Init(effectPotPin2, INPUT, led2Value);
    knob3.Init(effectPotPin3, INPUT, led3Value);

    // Initialize the LEDs
    pinMode(effectLedPin1, OUTPUT);
    pinMode(effectLedPin2, OUTPUT);
    pinMode(effectLedPin3, OUTPUT);
}

void KnobTest::AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[audioOutChannel][i] = in[audioInChannel][i];
    }
}

void KnobTest::Cleanup()
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

void KnobTest::Loop()
{
    // Knob 1 controls intensity of LED 1
    if (knob1.SetNewValue(led1Value))
    {
        // Write the new value to the LED
        analogWrite(effectLedPin1, led1Value * LED_MAX_VALUE);

        debugPrint("Updated the LED 1 level to: ");
        debugPrintln(led1Value);
    }

    // Knob 2 controls intensity of LED 2
    if (knob2.SetNewValue(led2Value))
    {
        // Write the new value to the LED
        analogWrite(effectLedPin2, led2Value * LED_MAX_VALUE);

        debugPrint("Updated the LED 2 level to: ");
        debugPrintln(led2Value);
    }
}

String KnobTest::GetEffectName()
{
    return "Knob Test";
}