#pragma once
#include <Gamebuino-Meta.h>

class Game;

class Menu
{
public:
    static const char * _str_unlock_help;
    Menu(Game * game);
    ~Menu();

    void init();
    void update();
    void draw();
private:
    Game * _game;
    uint8_t _level;
    int16_t _angle;
    uint32_t _time;
    bool _unlocked[6];
    uint32_t _high_scores[6];

    uint32_t _scroll; // scroll the unlock text
};