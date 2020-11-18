#ifndef HWTEST_H
#define HWTEST_H

#include "DaisyDuino.h"
#include "../../include/IEffect.h"
#include "../../include/PedalConfig.h"

class HWTest: public IEffect
{
    public:
        void Setup(size_t pNumChannels);
        void Cleanup();
        void AudioCallback(float **in, float **out, size_t size);
        void Loop();
        String GetEffectName();

    private:
        size_t numChannels;
        const int LED_MAX_VALUE = 256;
        const int LED_MIN_VALUE = 0;
        const int audioInChannel = 0;
        const int audioOutChannel = 0;
};

#endif