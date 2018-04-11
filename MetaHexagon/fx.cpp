#include "fx.h"

GSFX::FX sfx_ok[] =
{
    //{GSFX::WaveType::SQUARE, 19968,0,8192,0,1000},
    {GSFX::WaveType::SQUARE, 19968,-21,4096,0,3000}
};

GSFX::FX sfx_err[] =
{
    {GSFX::WaveType::SQUARE, 19968,0,8192,0,1000},
    {GSFX::WaveType::SQUARE, 19968,-21,11008,0,3000}
};

GSFX::FX sfx_back[] =
{
    {GSFX::WaveType::SQUARE, 19968,0,4096,0,1000},
    {GSFX::WaveType::SQUARE, 19968,-21,8192,0,3000}
};

GSFX::FX sfx_explosion = {GSFX::WaveType::NOISE, 15000,-1,8192,12,80000};