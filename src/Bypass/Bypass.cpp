#include "Bypass.h"

void Bypass::Setup(size_t pNumChannels)
{
    numChannels = pNumChannels;

    // Initialize the LEDs and buttons
    pinMode(effectSPSTPin1, INPUT);
    pinMode(effectSPSTPin2, INPUT);
    pinMode(effectSPSTPin3, INPUT);
    pinMode(effectPotPin1, INPUT);
    pinMode(effectPotPin2, INPUT);
    pinMode(effectPotPin3, INPUT);
    pinMode(effectLedPin1, OUTPUT);
    pinMode(effectLedPin2, OUTPUT);
    pinMode(effectLedPin3, OUTPUT);
}

void Bypass::AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t chn = 0; chn < numChannels; chn++)
        {
            out[chn][i] = in[chn][i];
        }
    }
}

void Bypass::Cleanup()
{
    // Turn off the LEDs
    debugPrint("Turning off LEDs");
    digitalWrite(effectLedPin1, LOW);
    digitalWrite(effectLedPin2, LOW);
    digitalWrite(effectLedPin3, LOW);
}

void Bypass::Loop()
{
    // Button1 turns on LED 1
    if (digitalRead(effectSPSTPin1) == HIGH)
    {
        digitalWrite(effectLedPin1, HIGH);
    }
    else
    {
        digitalWrite(effectLedPin1, LOW);
    }

    // Button 2 turns on LED 2
    if (digitalRead(effectSPSTPin2) == HIGH)
    {
        digitalWrite(effectLedPin2, HIGH);
    }
    else
    {
        digitalWrite(effectLedPin2, LOW);
    }

    // Knob 3 controls intensity of LED 3
    // Turn on the LED, at the level dictated by the knob
    //  (map the 10bit input value to an 8 bit PWM output)
    analogWrite(effectLedPin3, analogRead(effectPotPin3) / 4);
}