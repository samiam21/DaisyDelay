#include "NFNToggle.h"

void NFNToggle::Init(uint32_t pin1, uint32_t p1Mode, uint32_t pin2, uint32_t p2Mode)
{
    // Set and initialize the pins
    pinMode(pin1, p1Mode);
    pinMode(pin2, p2Mode);

    togglePin1 = pin1;
    togglePin2 = pin2;
}

uint8_t NFNToggle::ReadToggle()
{
    uint8_t ret = -1;

    // Read the pins
    int reading1 = digitalRead(togglePin1);
    int reading2 = digitalRead(togglePin2);

    // Check the position and return
    if (reading1 == HIGH)
    {
        ret = 0;
    }
    else if (reading2 == HIGH)
    {
        ret = 2;
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}