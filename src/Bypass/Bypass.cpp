#include "Bypass.h"

void Bypass::Setup(size_t pNumChannels)
{
    numChannels = pNumChannels;

    // Initialize the LEDs and buttons
    pinMode(effectSPSTPin1, INPUT);
    pinMode(effectSPSTPin2, INPUT);
    pinMode(effectSPSTPin3, INPUT);
    pinMode(effectSPDT1Pin1, INPUT);
    pinMode(effectSPDT1Pin2, INPUT);
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
    pinMode(effectLedPin1, OUTPUT);
    pinMode(effectLedPin2, OUTPUT);
    pinMode(effectLedPin3, OUTPUT);
    digitalWrite(effectLedPin1, LOW);
    digitalWrite(effectLedPin2, LOW);
    digitalWrite(effectLedPin3, LOW);
}

void Bypass::Loop()
{
    // SPDT controls whether the LEDs are turned on by buttons or POTs
    if (digitalRead(effectSPDT1Pin1) == HIGH)
    {
        // Button1 turns on LED 1
        if (digitalRead(effectSPSTPin1) == HIGH)
        {
            analogWrite(effectLedPin1, LED_MAX_VALUE);
        }
        else
        {
            analogWrite(effectLedPin1, LED_MIN_VALUE);
        }

        // Button 2 turns on LED 2
        if (digitalRead(effectSPSTPin2) == HIGH)
        {
            analogWrite(effectLedPin2, LED_MAX_VALUE);
        }
        else
        {
            analogWrite(effectLedPin2, LED_MIN_VALUE);
        }

        // Button 3 turns on LED 3
        if (digitalRead(effectSPSTPin3) == HIGH)
        {
            analogWrite(effectLedPin3, LED_MAX_VALUE);
        }
        else
        {
            analogWrite(effectLedPin3, LED_MIN_VALUE);
        }
    }
    else if (digitalRead(effectSPDT1Pin2) == HIGH)
    {
        // Knob 1 controls intensity of LED 1
        // Turn on the LED, at the level dictated by the knob
        //  (map the 10bit input value to an 8 bit PWM output)
        analogWrite(effectLedPin1, analogRead(effectPotPin1) / 4);

        // Knob 2 controls intensity of LED 2
        // Turn on the LED, at the level dictated by the knob
        //  (map the 10bit input value to an 8 bit PWM output)
        analogWrite(effectLedPin2, analogRead(effectPotPin2) / 4);

        // Knob 3 controls intensity of LED 3
        // Turn on the LED, at the level dictated by the knob
        //  (map the 10bit input value to an 8 bit PWM output)
        analogWrite(effectLedPin3, analogRead(effectPotPin3) / 4);
    }
}

String Bypass::GetEffectName()
{
    return "Bypass";
}