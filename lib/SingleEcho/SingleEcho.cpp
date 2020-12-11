#include "SingleEcho.h"

// TEMPO NOTES:
//  - Max delay size is based on audio rate, currently 96kHz
//  - Tempo = Max is equal to "slowest" delay
//    - 96000 => 30bpm
//    - 48000 => 60bpm
//    - 24000 => 120bpm
//    - Formula: 96000 / (bpm) * 30
//
//  - Timespan (between beats) -> Samples
//    - 2000ms = 96000
//    - 1000ms = 48000
//    - 500ms = 24000
//    - Formula: (96000 * t) / 2000

// Initialize the delay
void SingleEcho::Setup(size_t pNumChannels)
{
    // Init Delay Line
    del_line.Init();

    // Set Delay Time in Samples
    currentTempoSamples = ((96000 / initialTempoBpm) * 30) * tempoModifier;
    del_line.SetDelay(currentTempoSamples);

    // Initialize the tap tempo button
    tapTempoButton.Init(
        tapTempoButtonPin, INPUT, [this]() { return TapTempoInterruptHandler(); }, RISING);

    // Initialize the decay
    decay.Init(decayKnobPin, INPUT, decayValue, minDecayValue, maxDecayValue);

    // Initialize the level
    effectLevel.Init(levelKnobPin, INPUT, levelValue, minLevelValue, maxLevelValue);

    // Initialize the volume boost
    volumeBoost.Init(volumeBoostPin, INPUT, volumeBoostLevel);

    // Initialize the type pins
    typeSwitcher.Init(typeSwitcherPin1, INPUT, typeSwitcherPin2, INPUT);
    pinMode(quarterDelayLedPin, OUTPUT);
    pinMode(dottedEighthLedPin, OUTPUT);
    pinMode(tripletLedPin, OUTPUT);

    // Initialize the type
    TypeSwitcherLoopControl();
}

// Clean up the parameters for mono delay
void SingleEcho::Cleanup()
{
    del_line.Reset();
}

// Audio callback when audio input occurs
void SingleEcho::AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        float dry, wet;

        // Read Dry from I/O
        dry = in[audioInChannel][i];

        // Read Wet from Delay Line
        wet = del_line.Read();

        // Write to Delay with a controlled decay time
        del_line.Write((wet * decayValue) + dry);

        // Mix Dry and Wet and send to I/O
        out[audioOutChannel][i] = ((wet * levelValue) + dry) * (volumeBoostLevel * boostMultiplier + boostAdder);
    }
}

// Logic for mono delay to add into the main loop
void SingleEcho::Loop()
{
    // Update the decay if the knob has been moved
    if (decay.SetNewValue(decayValue))
    {
        debugPrint("Updated the decay to: ");
        debugPrintln(decayValue);
    }

    // Update the effect level if the knob has been moved
    if (effectLevel.SetNewValue(levelValue))
    {
        debugPrint("Updated the effect level to: ");
        debugPrintln(levelValue);
    }

    // Update the volume boost level if the knob has been moved
    if (volumeBoost.SetNewValue(volumeBoostLevel))
    {
        debugPrint("Updated the volume boost level to: ");
        debugPrintln(volumeBoostLevel);
    }

    // Handle type
    TypeSwitcherLoopControl();
}

// Interrupt handler for the tap tempo button to set the tempo
void SingleEcho::TapTempoInterruptHandler()
{
    debugPrintln("tap tempo button pressed");

    // Calculate the duration (ignore a duration longer than 2 seconds)
    unsigned long duration = millis() - tapTempoTime;
    if (duration < 2000)
    {
        // Add the duration to the tempo array (cast is safe because duration will never be greater than 2000)
        tempoArray.push(duration);

        // Calculate the average duration of the items in the array
        unsigned long avg = tempoArray.average();

        // Set the new delay based on the calculated duration
        currentTempoSamples = ((96000 * (size_t)avg) / 2000) * tempoModifier;
        del_line.SetDelay(currentTempoSamples);
    }
    else
    {
        // Duration was too long, reset the array for new tempo calculations
        tempoArray.clear();
        //debugPrint("Cleared the tempo array");
    }

    // Update the time
    tapTempoTime = millis();
}

// Return the effect name (for debugging)
String SingleEcho::GetEffectName()
{
    return "SingleEcho";
}

// Handle reading the SPDT switch and setting the delay type
void SingleEcho::TypeSwitcherLoopControl()
{
    // Determine which type is selected
    if (typeSwitcher.ReadToggle() == 0)
    {
        // Only set the type if we have a new one
        if (currentDelayType != QUARTER)
        {
            debugPrintln("Changing to quarter note delay");

            // Set the delay type and tempo modifier
            currentDelayType = QUARTER;
            tempoModifier = 1.0f;

            // Update the delay tempo
            del_line.SetDelay(currentTempoSamples * tempoModifier);

            // Turn off other LEDs
            analogWrite(tripletLedPin, 0);
            analogWrite(dottedEighthLedPin, 0);

            // Turn on QUARTER LED
            analogWrite(quarterDelayLedPin, ledIntensity);
        }
    }
    else if (typeSwitcher.ReadToggle() == 2)
    {
        // Only set the type if we have a new one
        if (currentDelayType != TRIPLET)
        {
            debugPrintln("Changing to triplet note delay");

            // Set the delay type and tempo modifier
            currentDelayType = TRIPLET;
            tempoModifier = 0.333f;

            // Update the delay tempo
            del_line.SetDelay(currentTempoSamples * tempoModifier);

            // Turn off other LEDs
            analogWrite(quarterDelayLedPin, 0);
            analogWrite(dottedEighthLedPin, 0);

            // Turn on QUARTER LED
            analogWrite(tripletLedPin, ledIntensity);
        }
    }
    else
    {
        // Only set the type if we have a new one
        if (currentDelayType != DOTTED_EIGHTH)
        {
            debugPrintln("Changing to dotted eighth note delay");

            // Set the delay type and tempo modifier
            currentDelayType = DOTTED_EIGHTH;
            tempoModifier = 0.75f;

            // Update the delay tempo
            del_line.SetDelay(currentTempoSamples * tempoModifier);

            // Turn off other LEDs
            analogWrite(tripletLedPin, 0);
            analogWrite(quarterDelayLedPin, 0);

            // Turn on QUARTER LED
            analogWrite(dottedEighthLedPin, ledIntensity);
        }
    }
}