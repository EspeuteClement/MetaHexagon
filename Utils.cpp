#include "Utils.h"
#include <Gamebuino-Meta.h>


namespace Utils
{
    // Sin table for sinus values encoded as usigned byte.
    const uint8_t sin_table[] = {0,6,13,19,25,31,37,44,50,56,62,68,74,80,86,92,98,103,109,115,120,126,131,136,142,147,152,157,162,167,171,176,180,185,189,193,197,201,205,208,212,215,219,222,225,228,231,233,236,238,240,242,244,246,247,249,250,251,252,253,254,254,255,255};

    int16_t sin(int32_t angle)
    {
        uint16_t mult = 1;
        angle = (angle % 256 + 256) % 256;
        if (angle > 127)
        {
            angle = 255 - angle;
            mult = -1;
        }
        if (angle > 63)
        {
            angle = 127 - angle;
        }


        return sin_table[angle%64] * mult;
    }

    int16_t cos(int32_t angle)
    {
        return sin(angle+64);
    }

    void drawQuad(const Point & p1, const Point & p2, const Point & p3, const Point & p4)
    {
        gb.display.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
        gb.display.fillTriangle(p4.x, p4.y, p2.x, p2.y, p3.x, p3.y);
    }
}
