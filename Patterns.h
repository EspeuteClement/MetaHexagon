#pragma once
#include "Hexagon.h"
#define __PATTERN_OVERRIDE__ 1
namespace Patterns
{

    const Hexagon::Pattern level_1[] =
    {
        {   
            5,
            {
                {1  ,20,5},
                {2  ,20,5},
                {3  ,20,5},
                {4  ,20,5},
                {5  ,20,5}
            }
        },
        {
            9,
            {
                {1  ,20,5},
                {3  ,20,5},
                {5  ,20,5},
                {0  ,35,5},
                {2  ,35,5},
                {4  ,35,5},
                {1  ,50,5},
                {3  ,50,5},
                {5  ,50,5},
            }
        }
    };

    inline const Hexagon::Pattern * getRandomPattern()
    {
#ifdef __PATTERN_OVERRIDE__
        return &level_1[__PATTERN_OVERRIDE__];
#else
        return &level_1[rand()%(sizeof(level_1)/sizeof(level_1))];
#endif
    }
}

