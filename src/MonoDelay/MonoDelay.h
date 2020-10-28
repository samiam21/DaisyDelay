#ifndef MONO_DELAY
#define MONO_DELAY

#include "DaisyDuino.h"
#include "../../PedalConfig.h"
#include "../IEffect.h"
#include "TempoArray.h"

/**********************************************
 * Mono Delay Effect
 * 
 * Button 1 - Tap Tempo
 * Button 2 - Type Switcher
 * Button 3 - N/U
 * 
 * Knob 1 - Effect Level
 * Knob 2 - Decay
 * Knob 3 - Mix
 * 
 * LED 1 - Quarter
 * LED 2 - Dotted Eighth
 * LED 3 - Triplet
 **********************************************/

// Pin renaming
static const int tapTempoButtonPin = effectSPSTPin1;
static const int levelKnobPin = effectPotPin1;
static const int decayKnobPin = effectPotPin2;

// Constant parameters
static const int audioInChannel = 0;
static const int audioOutChannel = 0;
static const size_t delayMaxSize = 96000;

// Tap tempo constants
static const long tapTempoDebounce = 300;

// Decay constants
static const int decayKnobFlutter = 10;
static const int minDecayKnobValue = 0;
static const int maxDecayKnobValue = 1024;
static const float maxDecayValue = 0.75f;

// Level constants
static const int levelKnobFlutter = 10;
static const int minLevelKnobValue = 0;
static const int maxLevelKnobValue = 1024;
static const float maxLevelValue = 1.0f;

class MonoDelay: public IEffect
{
    public:
        void Setup(size_t pNumChannels);
        void Cleanup();
        void AudioCallback(float **in, float **out, size_t size);
        void Loop();

    private:
        void TapTempoLoopControl();
        void DecayLoopControl();
        void SetDecayValue(int knobReading);
        void LevelLoopControl();
        void SetLevelValue(int knobReading);

        // Mutable parameters
        static DelayLine<float, delayMaxSize> del_line;

        // Tap tempo mutables
        size_t tempoBpm = 90;
        unsigned long tapTempoTime = 0;
        TempoArray tempoArray;

        // Decay mutables
        int decayKnobReading = 0;
        float decayValue = 0.5f;

        // Level mutables
        int levelKnobReading = 0;
        float levelValue = 0.5f;
};

#endif