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
    pinMode(tapTempoButtonPin, INPUT);
    pinMode(decayKnobPin, INPUT);

    // Initialize the decay
    decayKnobReading = analogRead(decayKnobPin);
    SetDecayValue(decayKnobReading);

    // Initialize the level
    levelKnobReading = analogRead(levelKnobPin);
    SetLevelValue(levelKnobReading);

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
        out[audioOutChannel][i] = (wet * levelValue) + dry;
    }
}

// Logic for mono delay to add into the main loop
void SingleEcho::Loop()
{
    // Handle tap tempo
    TapTempoLoopControl();

    // Handle decay
    DecayLoopControl();

    // Handle level
    LevelLoopControl();

    // Handle type
    TypeSwitcherLoopControl();
}

// Handle reading the decay knob and setting the decay
void SingleEcho::DecayLoopControl()
{
    // Read the decay knob
    int newDecayKnobReading = analogRead(decayKnobPin);

    // Account for flutter so we aren't constantly changing the decay
    if (newDecayKnobReading > (decayKnobReading + decayKnobFlutter) || newDecayKnobReading < (decayKnobReading - decayKnobFlutter))
    {
        // Check for min value, accounting for flutter
        if (newDecayKnobReading <= (minDecayKnobValue + decayKnobFlutter))
        {
            // Set the decay to min
            decayKnobReading = minDecayKnobValue;
            //debugPrint("MIN!");
        }
        // Check for max value, accounting for flutter
        else if (newDecayKnobReading >= (maxDecayKnobValue - decayKnobFlutter))
        {
            // Set the decay to max
            decayKnobReading = maxDecayKnobValue;
            //debugPrint("MAX!");
        }
        // Standard reading
        else
        {
            decayKnobReading = newDecayKnobReading;
            //debugPrint(newDecayKnobReading);
        }

        // Set the new decay value
        SetDecayValue(newDecayKnobReading);
        //debugPrint(decayValue);
    }
}

// Sets the decay value based on the passed in knob reading
void SingleEcho::SetDecayValue(int knobReading)
{
    decayValue = ((float)knobReading / (float)maxDecayKnobValue) * maxDecayValue;
}

// Handle reading the level knob and setting the level
void SingleEcho::LevelLoopControl()
{
    // Read the level knob
    int newLevelKnobReading = analogRead(levelKnobPin);

    // Account for flutter so we aren't constantly changing the level
    if (newLevelKnobReading > (levelKnobReading + levelKnobFlutter) || newLevelKnobReading < (levelKnobReading - levelKnobFlutter))
    {
        // Check for min value, accounting for flutter
        if (newLevelKnobReading <= (minLevelKnobValue + levelKnobFlutter))
        {
            // Set the level to min
            levelKnobReading = minLevelKnobValue;
            //debugPrint("MIN!");
        }
        // Check for max value, accounting for flutter
        else if (newLevelKnobReading >= (maxLevelKnobValue - levelKnobFlutter))
        {
            // Set the level to max
            levelKnobReading = maxLevelKnobValue;
            //debugPrint("MAX!");
        }
        // Standard reading
        else
        {
            levelKnobReading = newLevelKnobReading;
            //debugPrint(newLevelKnobReading);
        }

        // Set the new level value
        SetLevelValue(newLevelKnobReading);
        //debugPrint(levelValue);
    }
}

// Sets the level value based on the passed in knob reading
void SingleEcho::SetLevelValue(int knobReading)
{
    levelValue = ((float)knobReading / (float)maxLevelKnobValue) * maxLevelValue;
}

// Handle reading the tap tempo button and setting the tempo
void SingleEcho::TapTempoLoopControl()
{
    // Read the tap tempo button
    int reading = digitalRead(tapTempoButtonPin);

    // Debounce the button and check for it pressed
    if (reading == HIGH && millis() - tapTempoTime > tapTempoDebounce)
    {
        //debugPrint("tap tempo button pressed");

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
            debugPrint("Changing to quarter note delay");

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
            debugPrint("Changing to triplet note delay");

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
            debugPrint("Changing to dotted eighth note delay");

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