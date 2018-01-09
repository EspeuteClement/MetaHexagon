#pragma once
#include <Gamebuino-Meta.h>

class Game;

class Title {
public:
    Title(Game * game);

    void init();
    void update();
    void draw();
private:
    uint32_t _time;
    Game * _game;
};