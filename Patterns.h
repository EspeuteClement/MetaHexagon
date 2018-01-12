#include "Hexagon.h"
#include "Utils.h"
#include <Gamebuino-Meta.h>

#pragma once

//#define __PATTERN_OVERRIDE__ 9
namespace Patterns
{

    extern const Hexagon::Pattern patterns[];

    inline const Hexagon::Pattern * const getRandomPattern(const Hexagon::Level & level)
    {
#ifdef __PATTERN_OVERRIDE__
        return &(patterns[__PATTERN_OVERRIDE__]);
#else
        return (level.patterns[rand()%level.nb_patterns]);
#endif
    }

    extern const Hexagon::Level level_1;

    extern const Hexagon::Level level_2;

    extern const Hexagon::Level * levels[];
    const uint8_t LEVEL_COUNT = 6;
    void colorCallback_test(Color & bg1, Color & bg2, Color & walls, uint32_t time);
    void colorCallback_gray(Color & bg1, Color & bg2, Color & walls, uint32_t time);
    void colorCallback_black(Color & bg1, Color & bg2, Color & walls, uint32_t time);
    void colorCallback_lock(Color & bg1, Color & bg2, Color & walls, uint32_t time);
}



