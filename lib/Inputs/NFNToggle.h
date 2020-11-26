#ifndef NFNTOGGLE_H
#define NFNTOGGLE_H

#include <Arduino.h>
#include "DaisyDuino.h"
#include "../../include/PedalConfig.h"

/**
 * On/Off/On toggle class to handle reading the value of a toggle switch
 * This class will initialize the provided pins in the init function
 */
class NFNToggle
{
public:
    /**
     * These functions will initialize the toggle pins
     */
    void Init(uint32_t pin1, uint32_t p1Mode, uint32_t pin2, uint32_t p2Mode);

    /**
     * Reads the toggle pins and reports back the position
     * @return Position value, either 0, 1, or 2
     */
    uint8_t ReadToggle();

private:
    // Class variables
    int togglePin1 = -1;
    int togglePin2 = -1;
};

#endif