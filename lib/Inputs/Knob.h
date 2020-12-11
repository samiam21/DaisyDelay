#ifndef KNOB_H
#define KNOB_H

#include <Arduino.h>
#include "DaisyDuino.h"
#include "../../include/PedalConfig.h"

/**
 * Knob class to handle reading a knob value while accounting for jitter
 * This class will initialize the provided pin in the init function
 */
class Knob
{
public:
    /**
     * These functions will initialize the knob and set the initial value
     */
    void Init(uint32_t pin, uint32_t pMode, float &valueToSet);
    void Init(uint32_t pin, uint32_t pMode, float &valueToSet, float pMinValue, float pMaxValue);

    /**
     * This function will set a new value to the float passed in
     * If the knob value has not changed, or is within the jitter 
     * threshold, the value will not be changed.
     * @param valueToSet
     * @return Returns true if a new value was set, false if not
     */
    bool SetNewValue(float &valueToSet);

private:
    float GetNewValue(int newKnobReading);

    // Knob constants
    const int knobJitter = 10;
    const int minKnobReadingValue = 0;
    const int maxKnobReadingValue = 1024;

    // Class variables
    int knobPin = -1;
    float maxValue = 1.0f;
    float minValue = 0.0f;
    int knobReading = 0;
};

#endif