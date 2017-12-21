#pragma once
#include "Hexagon.h"
#define __PATTERN_OVERRIDE__ 3
namespace Patterns
{

    const Hexagon::Pattern level_1[] =
    {
        {   
            5, // All but one wall
            {
                {1  ,20,5},
                {2  ,20,5},
                {3  ,20,5},
                {4  ,20,5},
                {5  ,20,5}
            }
        },
        {   // 3, then 3, then 3 again  
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
        },
        {   // 2 big walls, zig zag for the others
            8,
            {
                {0, 20,35},
                {3, 20,35},
                {2, 20, 5},
                {5, 20, 5},
                {1, 35, 5},
                {4, 35, 5},
                {2, 50, 5},
                {5, 50, 5}
            }
        },
        {   // Spiral
            12,
            {
                {0,20, 10},
                {3,20, 10},

                {1,30, 10},
                {4,30, 10},

                {2,40, 10},
                {5,40, 10},

                {3,50, 10},
                {0,50, 10},

                {4,60, 10},
                {1,60, 10},

                {5,70, 10},
                {2,70, 10},

/*                {0,80, 10},
                {3,80, 10},

                {1,90, 10},
                {4,90, 10},

                {2,100, 10},
                {5,100, 10},

                {3,110, 10},
                {0,110, 10},

                {4,120, 10},
                {1,120, 10},

                {5,130, 10},
                {2,130, 10},*/
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

