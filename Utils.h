#pragma once
#include <cstdint>
#include "Config.h"

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
        union {uint8_t b;uint8_t v;int8_t z;};
    };

    vec3 hsv2rgb(vec3 hvs);
}

