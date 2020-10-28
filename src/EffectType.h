#ifndef EFFECT_TYPE_H
#define EFFECT_TYPE_H

#include "DaisyDuino.h"
#include "IEffect.h"
#include "Bypass\Bypass.h"

// Effect Objects
Bypass bypass;

/**
 * The rotary encoder is using Gray code, not standard hex.
 * The sequence of decimal numbers that it produces is as follows:
 * 0, 1, 3, 2, 6, 7, 5, 4, 12, 13, 15, 14, 10, 11, 9, 8
 */
enum EffectType {
    BYPASS = 0,

    // Add effect types here

    UNSET = 99
};

/**
 * Returns the effect object based on the passed in enum
 */
extern IEffect* GetEffectObject(EffectType type)
{
    switch(type)
    {
        case BYPASS:
        case UNSET:
        default:
            return (IEffect*)&bypass;
    }
};

#endif