#include "SFXR.h"
#include <Gamebuino-Meta.h>

SFXR::Sound_Handler_SFXR SFXR::_handler;
int SFXR::divider = 4;
int SFXR::freq = 2;
SFXR::SFX_Play SFXR::_sfx_play_pool[NUM_SFX];
uint8_t SFXR::_sfx_play_pool_head = 0;

// Empty constructors (they are never called)
SFXR::SFXR(){}
SFXR::~SFXR(){}


bool SFXR::init()
{
    bool is_init = gb.sound.play(&_handler, true);
    _handler.init();
    for (int i = 0; i < NUM_SFX; i++)
    {
        SFXR::_sfx_play_pool[i].time = UINT16_MAX;
    }

    return is_init;
}


// === SFX ===
SFXR::SFX::SFX(uint8_t len):_len(len), _type(SFXType::NOISE)
{

}

void SFXR::SFX::play()
{
    SFX_Play * p = &(SFXR::_sfx_play_pool[_sfx_play_pool_head]);
    p->time = 0;
    p->len = _len;
    p->type = _type;
    p->times = 4;

    _sfx_play_pool_head = (_sfx_play_pool_head+1)%NUM_SFX;
}



// === Sound_Handler_SFXR ===

uint8_t SFXR::Sound_Handler_SFXR::_buffer[NUM_SAMPLES];

SFXR::Sound_Handler_SFXR::Sound_Handler_SFXR(): Gamebuino_Meta::Sound_Handler(NULL)
{

}

SFXR::Sound_Handler_SFXR::~Sound_Handler_SFXR()
{

}

bool SFXR::Sound_Handler_SFXR::init()
{
    uint _head_index = 0;
    memset(SFXR::Sound_Handler_SFXR::_buffer, 0, NUM_SAMPLES);
    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        //SFXR::Sound_Handler_SFXR::_buffer[i] = rand(4096);
    }
    channel->index = 0;
    channel->total = NUM_SAMPLES;
    channel->last = false;
    channel->use = true;
    channel->buffer = &SFXR::Sound_Handler_SFXR::_buffer[0];
}

void SFXR::Sound_Handler_SFXR::update()
{
    channel->last = false;
    fillSamples();
}

void SFXR::Sound_Handler_SFXR::fillSamples() 
{
    uint32_t target = channel->index;
    uint32_t value = 0;
    int32_t samples_held = 0;
    /*do
    {
        samples_held --;
        if (samples_held <= 0)
        {
            value = !value;
            samples_held = rand(0) % SFXR::divider;
        }
        _head_index = (_head_index+1)%(NUM_SAMPLES>>2);
        _sample_time +=4;
        ((uint32_t*)(SFXR::Sound_Handler_SFXR::_buffer))[_head_index] = value * UINT32_MAX;
    }
    while (_head_index != target>>2);*/

    // GAMEBOY NOISE GENERATION, TAKEN FROM 
    // https://github.com/libretro/bsnes-libretro-cplusplus98/blob/master/gameboy/apu/noise/noise.cpp
    
    // Check if channels are alive
    bool alive = false;


    static uint32_t period = 1;
    uint32_t divisor = SFXR::divider;
    uint32_t frequency = SFXR::freq;
    static uint16_t lfsr = 1;
    do
    {
        uint32_t volume = 0;
        for (int i = 0; i < NUM_SFX; i++)
        {
            SFX_Play * p = &(SFXR::_sfx_play_pool[i]);
            if (p->time < UINT16_MAX)
            {
                p->time ++;
                if ((1 << p->len) < p->time)
                {
                    p->time = UINT16_MAX;
                }
                else
                {
                    volume = (((255- (((p->time*255)>>p->len)%256)) * (255-(((p->time*256*p->times)>>p->len)%256))))>> 9;
                }
            }
        }

        if(--period == 0) 
        {
            period = divisor << ((frequency - 2) % 16);
            if(((frequency - 2) % 16) < 14) 
            {
              bool bit = (lfsr ^ (lfsr >> 1)) & 1;
              lfsr = (lfsr >> 1) ^ (bit << 14);
            }
        }
        uint32_t sample = (lfsr & 1) ? 0 : (volume << 24) | (volume << 16) | (volume << 8) | volume;


        _head_index = (_head_index+1)%(NUM_SAMPLES>>2);
        //_sample_time +=4;
        ((uint32_t*)(SFXR::Sound_Handler_SFXR::_buffer))[_head_index] = sample;
    }
    while (_head_index != target>>2);

}

void SFXR::Sound_Handler_SFXR::rewind()
{

}

uint32_t SFXR::Sound_Handler_SFXR::getPos()
{
    return UINT32_MAX;
}

