#pragma once
#include "Hexagon.h"

namespace Patterns
{

    const Hexagon::Pattern level_1[] =
    {
        {5,
            {
                {1  ,20,5},
                {2  ,20,5},
                {3  ,20,5},
                {4  ,20,5},
                {5  ,20,5}
            }
        }
    };

    inline const Hexagon::Pattern * getRandomPattern()
    {
        return &level_1[rand()%(sizeof(level_1)/sizeof(level_1))];
    }
}

