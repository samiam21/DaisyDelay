#ifndef SINGLE_ECHO
#define SINGLE_ECHO

#include "DaisyDuino.h"
#include "../../include/IEffect.h"
#include "../../include/PedalConfig.h"
#include "TempoArray.h"
#include "../Inputs/NFNToggle.h"
#include "../Inputs/Knob.h"

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
 * Knob 3 - N/U
 * Knob 4 - N/U
 * 
 * LED 1 - Quarter
 * LED 2 - Dotted Eighth
 * LED 3 - Triplet
 * LED 4 - N/U
 **********************************************/

// Pin renaming
static const int tapTempoButtonPin = effectSPSTPin1;
static const int levelKnobPin = effectPotPin1;
static const int decayKnobPin = effectPotPin2;
static const int typeSwitcherPin1 = effectSPDT1Pin1;
static const int typeSwitcherPin2 = effectSPDT1Pin2;
static const int quarterDelayLedPin = effectLedPin1;
static const int dottedEighthLedPin = effectLedPin2;
static const int tripletLedPin = effectLedPin3;

// Constant parameters
static const size_t delayMaxSize = 96000;
static const size_t ledIntensity = 128;

// Tap tempo constants
static const long tapTempoDebounce = 300;
static const size_t initialTempoBpm = 90;

// Decay constants
static const int decayKnobFlutter = 10;
static const int minDecayKnobValue = 0;
static const int maxDecayKnobValue = 1024;
static const int minDecayValue = 0.0f;
static const float maxDecayValue = 0.75f;

// Level constants
static const int levelKnobFlutter = 10;
static const int minLevelKnobValue = 0;
static const int maxLevelKnobValue = 1024;
static const float maxLevelValue = 1.0f;

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
    void TapTempoLoopControl();
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

    // Mutable parameters
    DelayLine<float, delayMaxSize> del_line;

    // Tap tempo mutables
    size_t currentTempoSamples;
    unsigned long tapTempoTime = 0;
    TempoArray tempoArray;

    // Decay mutables
    int decayKnobReading = 0;
    float decayValue = 0.5f;

    // Level mutables
    int levelKnobReading = 0;
    float levelValue = 0.5f;

    // Type switcher mutables
    DelayType currentDelayType = DT_UNSET;
    float tempoModifier = 1.0f;
};

#endif