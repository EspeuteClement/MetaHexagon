#include "Sound.h"

uint8_t buffer[440];

Sound_Handler_Custom::Sound_Handler_Custom(Gamebuino_Meta::Sound_Channel* chan) : Gamebuino_Meta::Sound_Handler(chan) {
    // Fill our buffer
    for (int i = 0; i < 440; i++)
    {
        buffer[i] = (i < 220 ? 0xFF : 0x80);
    }
#ifdef __DEBUG_OUTPUT__
    SerialUSB.printf("[LOG] Sound create\n");
#endif
};

void Sound_Handler_Custom::update() {
    channel->index = 0;
    channel->total = 440;
    channel->buffer = &(buffer[0]);
    channel->type = Gamebuino_Meta::Sound_Channel_Type::raw;
    channel->loop = true;
    channel->last = false;

    channel->use = true;

#ifdef __DEBUG_OUTPUT__
    SerialUSB.printf("[LOG] Sound update\n");
#endif
}

void Sound_Handler_Custom::rewind() {
    
    channel->index = 0;
    channel->total = 440;
    channel->buffer = &(buffer[0]);
    channel->type = Gamebuino_Meta::Sound_Channel_Type::raw;
    channel->loop = true;
    channel->last = false;
    
    channel->use = true;

#ifdef __DEBUG_OUTPUT__
    SerialUSB.printf("[LOG] Sound update\n");
#endif
}