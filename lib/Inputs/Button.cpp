#include "Button.h"

void Button::Init(uint32_t pin, uint32_t pMode)
{
    // Set the button pin for the class
    buttonPin = pin;

    // Configure the button pin with the specific mode
    pinMode(buttonPin, pMode);
}

void Button::Init(uint32_t pin, uint32_t pMode, callback_function_t callback, uint32_t pInterruptMode)
{
    // Set the button pin for the class
    buttonPin = pin;

    // Configure the button pin with the specific mode
    pinMode(buttonPin, pMode);

    // Attach the local interrupt handler
    attachInterrupt(
        buttonPin, [this, callback]() { return LocalInterruptHandler(callback); }, pInterruptMode);
}

void Button::DetachInterrupt()
{
    // Detach the interrupt
    detachInterrupt(digitalPinToInterrupt(buttonPin));
}

bool Button::IsPressed(bool debounce)
{
    bool ret = false;

    // Read the button
    int reading = digitalRead(buttonPin);

    // Check for debounce command
    if (debounce)
    {
        // Debounce the button and check for it pressed
        if (millis() - lastButtonPress > buttonDebounce)
        {
            // Update last pressed time and set the return
            lastButtonPress = millis();
            ret = (reading == HIGH);
        }
    }
    else
    {
        // Check if the button is pressed
        ret = (reading == HIGH);
    }

    return ret;
}

void Button::LocalInterruptHandler(callback_function_t callback)
{
    // Debounce the button and check for it pressed
    if (millis() - lastButtonPress > buttonDebounce)
    {
        // Update last pressed time and call the callback
        lastButtonPress = millis();
        callback();
    }
}