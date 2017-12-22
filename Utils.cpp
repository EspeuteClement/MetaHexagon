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



    vec3 hsv2rgb(vec3 hsv)
    {
        vec3 rgb;
        unsigned char region, remainder, p, q, t;

        if (hsv.s == 0)
        {
            rgb.r = hsv.v;
            rgb.g = hsv.v;
            rgb.b = hsv.v;
            return rgb;
        }

        region = hsv.h / 43;
        remainder = (hsv.h - (region * 43)) * 6; 

        p = (hsv.v * (255 - hsv.s)) >> 8;
        q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
        t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

        switch (region)
        {
            case 0:
                rgb.r = hsv.v; rgb.g = t; rgb.b = p;
                break;
            case 1:
                rgb.r = q; rgb.g = hsv.v; rgb.b = p;
                break;
            case 2:
                rgb.r = p; rgb.g = hsv.v; rgb.b = t;
                break;
            case 3:
                rgb.r = p; rgb.g = q; rgb.b = hsv.v;
                break;
            case 4:
                rgb.r = t; rgb.g = p; rgb.b = hsv.v;
                break;
            default:
                rgb.r = hsv.v; rgb.g = p; rgb.b = q;
                break;
        }

        return rgb;     
    }



}
