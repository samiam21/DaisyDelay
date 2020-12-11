#include "ButtonTest.h"

void ButtonTest::Setup(size_t pNumChannels)
{
    numChannels = pNumChannels;

    // Initialize the buttons
    button1.Init(effectSPSTPin1, INPUT);
    button2.Init(effectSPSTPin2, INPUT);
    button3.Init(
        effectSPSTPin3, INPUT, [this]() { return Button3Interrupt(); }, RISING);

    // Initialize the LEDs
    pinMode(effectLedPin1, OUTPUT);
    pinMode(effectLedPin2, OUTPUT);
    pinMode(effectLedPin3, OUTPUT);
}

void ButtonTest::AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[audioOutChannel][i] = in[audioInChannel][i];
    }
}

void ButtonTest::Cleanup()
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

void ButtonTest::Loop()
{
    // Button1 turns on LED 1
    if (button1.IsPressed(false))
    {
        analogWrite(effectLedPin1, LED_MAX_VALUE);
    }
    else
    {
        analogWrite(effectLedPin1, LED_MIN_VALUE);
    }

    // Button 2 turns on LED 2
    if (button2.IsPressed(false))
    {
        analogWrite(effectLedPin2, LED_MAX_VALUE);
    }
    else
    {
        analogWrite(effectLedPin2, LED_MIN_VALUE);
    }
}

String ButtonTest::GetEffectName()
{
    return "Button Test";
}

void ButtonTest::Button3Interrupt()
{
    isLed3On = !isLed3On;
    digitalWrite(effectLedPin3, (int)isLed3On);
}