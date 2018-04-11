#pragma once
#include <cstdint>
#include "Config.h"
#include <Gamebuino-Meta.h>

/** Small mathematical library
*   @author Valden
*   @licence MIT
*/
namespace Utils
{
    /** Return the sine of *angle* as a signed int between -256 and 256. Value is 
    *   @param angle An angle exprimed in a 256 base. So 0 for 0PI, 128 for PI, 256 for 2PI
    */
    int16_t sin(int32_t angle);

    /** Return the cosine of *angle* as a signed int between -256 and 256. Value is 
    *   @param angle An angle exprimed in a 256 base. So 0 for 0PI, 128 for PI, 256 for 2PI
    *   @return the angle as a signed int between -256 (for -1.0) and 256 (for 1.0)
    */
    int16_t cos(int32_t angle);

    struct Point { int16_t x; int16_t y;};
    void drawQuad(const Point & p1, const Point & p2, const Point & p3, const Point & p4);

    /** Perform a modulo on value by div, and works even if value is negative */
    inline int32_t smod(int32_t value, int32_t div)
    {
        return ((value % div) + div) % div;
    }

    template <typename T> inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
    }

    struct vec3
    {
        union {uint8_t r;uint8_t h;int8_t x;};
        union {uint8_t g;uint8_t s;int8_t y;};
        union {uint8_t b;uint8_t v;uint8_t l; int8_t z;};
    };

    static const int32_t FPP = 5;

    // Convert an integer to a fixed point representation
    inline constexpr int32_t toFix(const int32_t a) { return a << FPP;};

    // Convert a fixed point number to a integer
    inline constexpr int32_t fromFix(const int32_t a) { return a >> FPP;};

    vec3 hsv2rgb(vec3 hvs);

    void computeLaneTrig(int16_t _angle_offset, uint8_t _sides);
    int32_t getLaneTrig(uint8_t index);

    inline Utils::Point getPoint(uint8_t lane, int16_t distance, uint8_t _sides)
    {
        // I tried to make this function as fast as possible.
        return 
        {
            ((getLaneTrig((lane%_sides) * 2) * distance) >> 8) + gb.display.width()/2, // x
            ((getLaneTrig((lane%_sides) * 2 + 1) * distance) >> 8) + gb.display.height()/2 // y
        };
    }

    vec3 hsl2rgb(const vec3 & hsl);
    uint8_t Hue2rgb(  uint16_t v1, uint16_t v2, uint8_t vH );

    void drawTextCenter(uint8_t x, uint8_t y, const char * str);
}

