#pragma once

#include <Gamebuino-Meta.h>

class Sound_Handler_Custom : public Gamebuino_Meta::Sound_Handler {
public:
    Sound_Handler_Custom(Gamebuino_Meta::Sound_Channel* chan);
    void update();
    void rewind();
private:
};