#ifndef IEFFECT_H
#define IEFFECT_H

class IEffect
{
    public:
        virtual void Setup(size_t pNumChannels);
        virtual void Cleanup();
        virtual void AudioCallback(float **in, float **out, size_t size);
        virtual void Loop();
        virtual String GetEffectName();
};

#endif