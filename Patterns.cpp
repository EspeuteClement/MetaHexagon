#include "Patterns.h"
#include "Utils.h"

void colorCallback_test(Color & bg1, Color & bg2, Color & walls, uint32_t time)
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
    Utils::vec3 color = Utils::hsv2rgb({time, 127, 127});
    bg1 = gb.createColor(color.r, color.g, color.b);
    color = Utils::hsv2rgb({time, 160, 160});
    bg2 = gb.createColor(color.r, color.g, color.b);
    color = Utils::hsv2rgb({time, 200, 200});
    walls = gb.createColor(color.r, color.g, color.b);

}