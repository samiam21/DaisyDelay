#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "DaisyDuino.h"
#include "../../include/PedalConfig.h"

/**
 * Button class to handle reading a button value while debouncing it
 * This class will initialize the provided pin in the init function
 */
class Button
{
public:
    /**
     * Initialize the button
     */
    void Init(uint32_t pin, uint32_t pMode);

    /**
     * Initialize the button in interrupt mode
     */
    void Init(uint32_t pin, uint32_t pMode, callback_function_t callback, uint32_t pInterruptMode);

    /**
     * Detaches the interrupt(s) that are configured in the init function
     */
    void DetachInterrupt();

    /**
     * Checks if the button is pressed, accounting for debounce
     * @return Returns true if the button is pressed, false if not
     */
    bool IsPressed(bool debounce = true);

private:
    void LocalInterruptHandler(callback_function_t callback);

    int buttonPin = -1;

    const unsigned long buttonDebounce = 200;
    unsigned long lastButtonPress = 0;
};

#endif