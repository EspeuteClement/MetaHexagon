#pragma once

#include <utility/Sound.h>

class SFXR {
public:
    // Max sfx that can be played at the same time
    static const int NUM_SFX = 1;

    /** Init the library, allowing it to play sounds
    */
    static bool init();
    
    /** Update the library. Call this function once per frame
    */
    static void update();

    enum class SFXType
    {
        NOISE,
        SQUARE,
    };


    class SFX
    {
    public:
        SFX(uint8_t len);
        void play();
    private:
        uint8_t _len;
        SFXType _type;
    };



    static int divider;
    static int freq;

private:
    SFXR();
    ~SFXR();

    class Sound_Handler_SFXR : public Gamebuino_Meta::Sound_Handler
    {
    public:
        static const int NUM_SAMPLES = 2048;
        Sound_Handler_SFXR();
        ~Sound_Handler_SFXR();
        bool init();
        void update();
        void rewind();
        void fillSamples() __attribute__((optimize("-O3")));
        inline uint32_t rand(uint16_t freq) 
        {
            static uint32_t state = 5;
            uint32_t n = (state);
            n ^= n << 13;
            n ^= n >> 17;
            n ^= n << 5;
            state = n;
            return n;
        };
        uint32_t getPos();
    private:
        uint32_t _sample_time;
        uint16_t _head_index;
        static uint8_t _buffer[NUM_SAMPLES];
    };

    struct SFX_Play
    {
        uint8_t len;
        uint8_t times;
        uint16_t time;
        SFXType type;
    };

    static Sound_Handler_SFXR _handler;
    static SFX_Play _sfx_play_pool[NUM_SFX];

    static uint8_t _sfx_play_pool_head;

};


