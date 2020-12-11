#include "Knob.h"

void Knob::Init(uint32_t pin, uint32_t pMode, float &valueToSet)
{
    Init(pin, pMode, valueToSet, minValue, maxValue);
}

void Knob::Init(uint32_t pin, uint32_t pMode, float &valueToSet, float pMinValue, float pMaxValue)
{
    // Set the pin to read
    knobPin = pin;

    // Update the new min and max values
    minValue = pMinValue;
    maxValue = pMaxValue;

    // Initialize the pin and set the initial knob reading
    pinMode(pin, pMode);
    knobReading = analogRead(knobPin);

    // Set the initial value
    valueToSet = GetNewValue(knobReading);
}

bool Knob::SetNewValue(float &valueToSet)
{
    bool ret = false;

    // Read the knob
    int newKnobReading = analogRead(knobPin);

    // Account for jitter so we aren't constantly changing the value
    if (newKnobReading > (knobReading + knobJitter) || newKnobReading < (knobReading - knobJitter))
    {
        // Update the value
        valueToSet = GetNewValue(newKnobReading);

        // A new value was set, return true
        ret = true;
    }

    return ret;
}

float Knob::GetNewValue(int newKnobReading)
{
    float newValue = minValue;

    // Check for min value, accounting for jitter
    if (newKnobReading <= (minKnobReadingValue + knobJitter))
    {
        // Set the reading to min
        knobReading = minKnobReadingValue;
        //debugPrintln("MIN!");
    }
    // Check for max value, accounting for jitter
    else if (newKnobReading >= (maxKnobReadingValue - knobJitter))
    {
        // Set the reading to max
        knobReading = maxKnobReadingValue;
        //debugPrintln("MAX!");
    }
    // Standard reading
    else
    {
        knobReading = newKnobReading;
        //debugPrintln(newKnobReading);
    }

    // Get the new value
    if (knobReading == maxKnobReadingValue)
    {
        newValue = maxValue;
    }
    else if (knobReading == minKnobReadingValue)
    {
        newValue = minValue;
    }
    else
    {
        newValue = ((float)knobReading / (float)maxKnobReadingValue) * (maxValue - minValue) + minValue;
    }

    // Return the new value
    return newValue;
}