#include "Patterns.h"
#include "Utils.h"


const Hexagon::Pattern Patterns::patterns[] =
    {
        {   
            5, // All but one wall
            {
                {1  ,30,5},
                {2  ,30,5},
                {3  ,30,5},
                {4  ,30,5},
                {5  ,30,5}
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

        /*      {0,80, 10},
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
        },
        {   
            15, // All but one wall * 3
            {
                {1  ,30,5},
                {2  ,30,5},
                {3  ,30,5},
                {4  ,30,5},
                {5  ,30,5},
                {4  ,60,5},
                {5  ,60,5},
                {0  ,60,5},
                {1  ,60,5},
                {2  ,60,5},
                {1  ,90,5},
                {2  ,90,5},
                {3  ,90,5},
                {4  ,90,5},
                {5  ,90,5},
            }
        },
        {   // 3 in quick succesion
            9,
            {
                {1  ,20,5},
                {3  ,20,5},
                {5  ,20,5},
                {1  ,32,5},
                {3  ,32,5},
                {5  ,32,5},
                {1  ,44,5},
                {3  ,44,5},
                {5  ,44,5}
            }
        },
        {   // Whirlpool
            15,
            {
                {1, 20, 25},
                {3, 20, 5},
                {4, 20, 10},
                {5, 20, 15},
                {6, 20, 20},
                {2, 45, 5},
                {3, 50, 5},
                {4, 55, 5},
                {5, 60, 5},
                {0, 65, 5},
                {1, 70, 5},
                {2, 80, 5},
                {3, 85, 5},
                {4, 90, 5},
                {5, 95, 5}
            }
        }
    };

const Hexagon::Level Patterns::level_1 =
    {
        0,
        Utils::toFix(8),
        Utils::toFix(1),
        {
            &patterns[0],
            &patterns[1],
            &patterns[2],
            &patterns[3],
        },
        4,
        &Patterns::colorCallback_test
    };

const Hexagon::Level Patterns::level_2 =
{
    1,
    Utils::toFix(10),
    Utils::toFix(1) + 16,
    {
        &patterns[0],
    },
    1,
    &Patterns::colorCallback_black
};

extern const Hexagon::Level * Patterns::levels[] = 
{&Patterns::level_1, &Patterns::level_2,&Patterns::level_1, &Patterns::level_2,&Patterns::level_1, &Patterns::level_2};




void Patterns::colorCallback_test(Color & bg1, Color & bg2, Color & walls, uint32_t time)
{
    /*if (time % 50 > 25)
    {
        bg1   = gb.createColor(18, 78, 137);
        bg2  = gb.createColor(0,149,233);
    }
    else
    {
        bg2   = gb.createColor(18, 78, 137);
        bg1  = gb.createColor(0,149,233);
    }*/

    /*walls = gb.createColor(44,232,245);*/
    Utils::vec3 color = Utils::hsl2rgb({time, 127, 100});
    bg1 = gb.createColor(color.r, color.g, color.b);
    
    color = Utils::hsl2rgb({time, 160, 127});
    bg2 = gb.createColor(color.r, color.g, color.b);
    
    color = Utils::hsl2rgb({time, 200, 200});
    walls = gb.createColor(color.r, color.g, color.b);
}

void Patterns::colorCallback_gray(Color & bg1, Color & bg2, Color & walls, uint32_t time)
{
    Utils::vec3 color = Utils::hsl2rgb({0, 0, 120});
    bg1 = gb.createColor(color.r, color.g, color.b);
    
    color = Utils::hsl2rgb({0, 0, 110});
    bg2 = gb.createColor(color.r, color.g, color.b);
    
    color = Utils::hsl2rgb({0, 0, 200});
    walls = gb.createColor(color.r, color.g, color.b);
}

void Patterns::colorCallback_lock(Color & bg1, Color & bg2, Color & walls, uint32_t time)
{
    Utils::vec3 color = Utils::hsl2rgb({0, 0, 75});
    bg1 = gb.createColor(color.r, color.g, color.b);
    
    color = Utils::hsl2rgb({0, 0, 95});
    bg2 = gb.createColor(color.r, color.g, color.b);
    
    color = Utils::hsl2rgb({0, 0, 180});
    walls = gb.createColor(color.r, color.g, color.b);
}

void Patterns::colorCallback_black(Color & bg1, Color & bg2, Color & walls, uint32_t time)
{
    if ((time/25) % 2)
    {
        Utils::vec3 color = Utils::hsl2rgb({time<<2, 50, 10});
        bg1 = gb.createColor(color.r, color.g, color.b);
        
        color = Utils::hsl2rgb({time<<2, 50, 50});
        bg2 = gb.createColor(color.r, color.g, color.b);
    }
    else
    {
        Utils::vec3 color = Utils::hsl2rgb({time<<2, 50, 50});
        bg1 = gb.createColor(color.r, color.g, color.b);
        
        color = Utils::hsl2rgb({time<<2, 50, 10});
        bg2 = gb.createColor(color.r, color.g, color.b);
    }
    
    Utils::vec3 color = Utils::hsl2rgb({time<<2, 100, 200});
    walls = gb.createColor(color.r, color.g, color.b);
}
