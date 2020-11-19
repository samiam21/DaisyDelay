#ifndef EFFECT_TYPE_H
#define EFFECT_TYPE_H

#include "DaisyDuino.h"
#include "IEffect.h"
#include "../lib/HWTest/HWTest.h"
#include "../lib/SingleEcho/SingleEcho.h"

// Effect Objects
HWTest hwTest;
SingleEcho singleEcho;

/**
 * The rotary encoder is using Gray code, not standard hex.
 * The sequence of decimal numbers that it produces is as follows:
 * 0, 1, 3, 2, 6, 7, 5, 4, 12, 13, 15, 14, 10, 11, 9, 8
 */
enum EffectType
{
    HWTEST = 0,
    SINGLEECHO = 1,

    // Possible future effects
    DOUBLEECHO = 3,
    DOUBLER = 4,

    UNSET = 99
};

/**
 * Returns the effect object based on the passed in enum
 */
extern IEffect *GetEffectObject(EffectType type)
{
    switch (type)
    {
    case SINGLEECHO:
        return (IEffect *)&singleEcho;
    case HWTEST:
    case UNSET:
    default:
        return (IEffect *)&hwTest;
    }
};

#endif