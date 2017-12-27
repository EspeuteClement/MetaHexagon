#pragma once
#include <Gamebuino-Meta.h>
#include "Hexagon.h"
#include "Config.h"
#include "Menu.h"

class Game
{
public:
    static const uint8_t MAX_STATE_STACK = 8;
    enum class State {
        MENU,
        HEXAGON,
        NONE
    };

    // Constructor / Destuctor
    Game();
    ~Game();

    // === ===

    // Initialise the game
    void init();

    // Does one tick of the game
    void update();

    // Draw the game
    void draw();

    // Start the game
    void startHexagon(uint8_t level_id);
    inline void pushState(State state) 
    { 
        _state_ptr ++;
        _state_stack[_state_ptr] = state;
    };
    inline void popState() { _state_ptr --;};

    void setScore(uint32_t score, uint8_t index);
    uint32_t getScore(uint8_t index);

private:
    Hexagon hexagon;
    Menu _menu;
    State _state_stack[MAX_STATE_STACK];
    uint8_t _state_ptr;
    uint32_t _scores[6];
};