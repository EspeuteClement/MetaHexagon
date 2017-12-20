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

}

