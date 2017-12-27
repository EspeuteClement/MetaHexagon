#include "Utils.h"
#include <Gamebuino-Meta.h>
#include "Hexagon.h"


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

    int32_t _lane_trig[Hexagon::MAX_SIDES * 2];

    void computeLaneTrig(int16_t _angle_offset, uint8_t _sides)
    {
        for (int i = 0; i < _sides; i++)
        {
            uint32_t angle = (i * 256/_sides + fromFix(_angle_offset));
            _lane_trig[i * 2]      = Utils::sin(angle);
            _lane_trig[i * 2  + 1] = -Utils::cos(angle);
        }
    }

    int32_t getLaneTrig(uint8_t index)
    {
        return _lane_trig[index];
    }




    // code adapted from http://www.easyrgb.com/en/math.php#text18
    vec3 hsl2rgb(const vec3 & hsl)
    {
        vec3 rgb;
        if ( hsl.s == 0 )
        {

           rgb.r = hsl.l;
           rgb.g = hsl.l;
           rgb.b = hsl.l;
        }
        else
        {
            uint16_t var_2 = 0;

           if ( hsl.l < 127 ) var_2 = (hsl.l * ( 256 + hsl.s ));
           else           var_2 = (( hsl.l + hsl.s ) << 8) - ( (uint16_t)hsl.s * (uint16_t)hsl.l );
           var_2 >>= 8;
           uint16_t var_1 = 2 * hsl.l - var_2;

           rgb.r = Hue2rgb( var_1, var_2, hsl.h + 85 );
           rgb.g = Hue2rgb( var_1, var_2, hsl.h );
           rgb.b = Hue2rgb( var_1, var_2, hsl.h - 85 );
        }
        return rgb;
    }


    uint8_t Hue2rgb( uint16_t v1, uint16_t v2, uint8_t vH )             //Function Hue_2_RGB
    {
        uint16_t v = vH;
       if ( ( 6 * v ) < 256 ) return (( (v1 << 8) + ( v2 - v1 ) * 6 * v ) >> 8);
       if ( ( 2 * v ) < 256 ) return ( v2 );
       if ( ( 3 * v ) < 512 ) return (( (v1 << 8) + ( v2 - v1 ) * ( 171 - v ) * 6 )) >> 8;
       return ( v1 );
    }
}

void Utils::drawTextCenter(uint8_t x, uint8_t y, const char * str)
{
    int len = strlen(str);
    gb.display.setCursorX(x - (len * 2));
    gb.display.setCursorY(y);
    gb.display.print(str);
}
