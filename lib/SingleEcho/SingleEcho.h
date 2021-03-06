#ifndef SINGLE_ECHO
#define SINGLE_ECHO

#include "DaisyDuino.h"
#include "../../include/IEffect.h"
#include "../../include/PedalConfig.h"
#include "TempoArray.h"
#include "../Inputs/NFNToggle.h"
#include "../Inputs/Knob.h"
#include "../Inputs/Button.h"

/**********************************************
 * Mono Delay Effect
 * 
 * SPST 1 - Tap Tempo
 * SPST 2 - N/U
 * SPST 3 - N/U
 * SPST 4 - N/U
 * 
 * SPDT 1 - Type Switcher
 * SPDT 2 - N/U
 * 
 * Knob 1 - Effect Level
 * Knob 2 - Decay
 * Knob 3 - Volume Boost
 * Knob 4 - N/U
 * 
 * LED 1 - Quarter
 * LED 2 - Dotted Eighth
 * LED 3 - Triplet
 * LED 4 - N/U
 **********************************************/

// Pin renaming
static const int tapTempoButtonPin = effectSPSTPin4;
static const int levelKnobPin = effectPotPin4;
static const int decayKnobPin = effectPotPin2;
static const int volumeBoostPin = effectPotPin3;
static const int typeSwitcherPin1 = effectSPDT2Pin1;
static const int typeSwitcherPin2 = effectSPDT2Pin2;
static const int quarterDelayLedPin = effectLedPin1;
static const int dottedEighthLedPin = effectLedPin2;
static const int tripletLedPin = effectLedPin3;

// Constant parameters
static const size_t delayMaxSize = 96000;
static const size_t ledIntensity = 128;

// Tap tempo constants
static const size_t initialTempoBpm = 90;

// Decay constants
static const float minDecayValue = 0.75f;
static const float maxDecayValue = 0.0f;

// Level constants
static const float minLevelValue = 1.0f;
static const float maxLevelValue = 0.0f;

// Volume boost constants
static const float boostMinValue = 7.0f;
static const float boostMaxValue = 1.0f;

// Type constants
enum DelayType
{
    QUARTER = 0,
    DOTTED_EIGHTH = 1,
    TRIPLET = 2,

    DT_UNSET = 99
};

class SingleEcho : public IEffect
{
public:
    void Setup(size_t pNumChannels);
    void Cleanup();
    void AudioCallback(float **in, float **out, size_t size);
    void Loop();
    String GetEffectName();

private:
    void TapTempoInterruptHandler();
    void DecayLoopControl();
    void LevelLoopControl();
    void TypeSwitcherLoopControl();
    void SetDecayValue(int knobReading);
    void SetLevelValue(int knobReading);
    void SetType();

    // Input handlers
    NFNToggle typeSwitcher;
    Knob effectLevel;
    Knob decay;
    Knob volumeBoost;
    Button tapTempoButton;

    // Mutable parameters
    DelayLine<float, delayMaxSize> del_line;
    float decayValue = 0.5f;
    float levelValue = 0.5f;
    float volumeBoostLevel = 0.0f;

    // Tap tempo mutables
    size_t currentTempoSamples;
    unsigned long tapTempoTime = 0;
    TempoArray tempoArray;

    // Type switcher mutables
    DelayType currentDelayType = DT_UNSET;
    float tempoModifier = 1.0f;
};

#endif