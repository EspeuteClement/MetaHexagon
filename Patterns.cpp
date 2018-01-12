#include "Patterns.h"
#include "Utils.h"


const Hexagon::Pattern Patterns::patterns[] =
    {
        {   
            5, // 0 All but one wall
            {
                {1  ,10,5},
                {2  ,10,5},
                {3  ,10,5},
                {4  ,10,5},
                {5  ,10,5}
            }
        },
        {   // 3, then 3, then 3 again  
            9,
            {
                #define S 20 // spacing between walls
                {1  ,10,5},
                {3  ,10,5},
                {5  ,10,5},
                {0  ,10+S,5},
                {2  ,10+S,5},
                {4  ,10+S,5},
                {1  ,10+2*S,5},
                {3  ,10+2*S,5},
                {5  ,10+2*S,5},
            }
        },
        {   // 1 : 2 big walls, zig zag for the others
            
            8,
            {
                #define O 20
                {0, 10,O*2 + 5},
                {3, 10,O*2 + 5},
                {2, 10, 5},
                {5, 10, 5},
                {1, 10 + O, 5},
                {4, 10 + O, 5},
                {2, 10 + O * 2, 5},
                {5, 10 + O * 2, 5}
                #undef O
            }
        },
        {   // 2 : Spiral
            12,
            {
                {0,10, 10},
                {3,10, 10},

                {1,20, 10},
                {4,20, 10},

                {2,30, 10},
                {5,30, 10},

                {3,40, 10},
                {0,40, 10},

                {4,50, 10},
                {1,50, 10},

                {5,60, 10},
                {2,60, 10},

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
            15, // 3 : All but one wall * 3
            {
                {1  ,10,5},
                {2  ,10,5},
                {3  ,10,5},
                {4  ,10,5},
                {5  ,10,5},
                {4  ,40,5},
                {5  ,40,5},
                {0  ,40,5},
                {1  ,40,5},
                {2  ,40,5},
                {1  ,70,5},
                {2  ,70,5},
                {3  ,70,5},
                {4  ,70,5},
                {5  ,70,5},
            }
        },
        {   // 4 : 3 in quick succesion
            9,
            {
                {1  ,10,5},
                {3  ,10,5},
                {5  ,10,5},
                {1  ,22,5},
                {3  ,22,5},
                {5  ,22,5},
                {1  ,34,5},
                {3  ,34,5},
                {5  ,34,5}
            }
        },
        {   // 5 :  Whirlpool
            15,
            {
                #define WO 6
                #define S 0
                {1, S + WO, WO*6},
                {3, S + WO, WO},
                {4, S + WO, WO*2},
                {5, S + WO, WO*3},
                {6, S + WO, WO*4},
                {2, S + 6*WO, WO},
                {3, S + 7*WO, WO},
                {4, S + 8*WO, WO},
                {5, S + 9*WO, WO},
                {0, S + 10*WO, WO},
                {1, S + 11*WO, WO},
                {2, S + 12*WO, WO},
                {3, S + 13*WO, WO},
                {4, S + 14 * WO, WO},
                {5, S + 15*WO, WO}
                #undef WO
                #undef S
            }
        }, 
        { // 7: , then 2 to avoid
            8,
            {
                #define O 10
                #define S 20
                {1, O, 5},
                {2, O, 5},
                {4, O, 5},
                {5, O, 5},
                {0, O+S,5},
                {1, O+S,5},
                {3,O+S,5},
                {4,O+S,5}
                #undef O
            }
        },
        { // 8: , then C with holes
            4,
            {
                #define O 10
                {1, O, 5},
                {2, O, 5},
                {3, O, 5},
                {5, O, 5},
                #undef O
            }
        },
        { // 8: , then C with holes
            8,
            {
                #define O 10
                #define S 25
                {1, O, 5},
                {2, O, 5},
                {3, O, 5},
                {5, O, S+5},
                {3, O+S, 5},
                {4, O+S, 5},
                {0, O+S, 5},
                {1, O+S, 5},
                #undef O
                #undef S
            }
        }
    };

int16_t angleSpeedCallbackLevel1(int16_t current_speed, uint32_t time)
{
    if (time == 0)
    {
        return Utils::toFix(1);
    }
    if (time % 127 == 0)
    {
        current_speed = -(current_speed * 6) / 5;
        if (abs(current_speed) >= Utils::toFix(2))   // Cap the max speed
        {
            current_speed = Utils::sign(current_speed) * Utils::toFix(2);
        }
    }
    return current_speed;
}

const Hexagon::Level Patterns::level_1 =
    {
        0,
        Utils::toFix(6),
        6,
        Utils::toFix(1)/2 + 8,
        2,
        {
            &patterns[0],
            &patterns[1],
            &patterns[2],
            &patterns[4],
            &patterns[5],
            &patterns[7],
        },
        4,
        &Patterns::colorCallback_test,
        &angleSpeedCallbackLevel1
    };

const Hexagon::Level Patterns::level_2 =
{
    1,
    Utils::toFix(10),
    4,
    Utils::toFix(1) + 16,
    4,
    {
        &patterns[0],
    },
    1,
    &Patterns::colorCallback_black,
    &angleSpeedCallbackLevel1
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
    Utils::vec3 color = Utils::hsl2rgb({(Utils::cos(time)>>4)+ (1 <<4), 127, 100});
    bg1 = gb.createColor(color.r, color.g, color.b);
    
    color = Utils::hsl2rgb({(Utils::cos(time)>>4)+ (1 <<4), 160, 127});
    bg2 = gb.createColor(color.r, color.g, color.b);
    
    color = Utils::hsl2rgb({(Utils::cos(time)>>4)+ (1 <<4), 170, 200});
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
        Utils::vec3 color = Utils::hsl2rgb({time<<2, 50, 20});
        bg1 = gb.createColor(color.r, color.g, color.b);
        
        color = Utils::hsl2rgb({time<<2, 50, 40});
        bg2 = gb.createColor(color.r, color.g, color.b);
    }
    else
    {
        Utils::vec3 color = Utils::hsl2rgb({time<<2, 50, 25});
        bg1 = gb.createColor(color.r, color.g, color.b);
        
        color = Utils::hsl2rgb({time<<2, 50, 35});
        bg2 = gb.createColor(color.r, color.g, color.b);
    }
    
    Utils::vec3 color = Utils::hsl2rgb({time<<2, 100, 200});
    walls = gb.createColor(color.r, color.g, color.b);
}
