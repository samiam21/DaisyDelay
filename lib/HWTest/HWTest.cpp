#include "HWTest.h"

void HWTest::Setup(size_t pNumChannels)
{
    numChannels = pNumChannels;

    // Initialize the buttons
    button1.Init(effectSPSTPin1, INPUT);
    button2.Init(effectSPSTPin2, INPUT);
    button3.Init(effectSPSTPin3, INPUT, [this](){ return Button3Interrupt(); }, RISING);

    // Initialize the knobs
    knob1.Init(effectPotPin1, INPUT, led1Value);
    knob2.Init(effectPotPin2, INPUT, led2Value);
    knob3.Init(effectPotPin3, INPUT, volumeBoost);

    // Initialize the toggle
    toggle.Init(effectSPDT1Pin1, INPUT, effectSPDT1Pin2, INPUT);

    // Initialize the LEDs
    pinMode(effectLedPin1, OUTPUT);
    pinMode(effectLedPin2, OUTPUT);
    pinMode(effectLedPin3, OUTPUT);
}

void HWTest::AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[audioOutChannel][i] = in[audioInChannel][i] * (volumeBoost * 4.0 + 1.0f);
    }
}

void HWTest::Cleanup()
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

void HWTest::Loop()
{
    // SPDT controls whether the LEDs are turned on by buttons or POTs
    if (toggle.ReadToggle() == 0)
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
    else if (toggle.ReadToggle() == 2)
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

    // Knob 3 controls the volume boost
    if (knob3.SetNewValue(volumeBoost))
    {
        debugPrint("Updated the volume boost level to: ");
        debugPrintln(volumeBoost);
    }
}

String HWTest::GetEffectName()
{
    return "Hardware Test";
}

void HWTest::Button3Interrupt()
{
    isLed3On = !isLed3On;
    digitalWrite(effectLedPin3, (int)isLed3On);
}