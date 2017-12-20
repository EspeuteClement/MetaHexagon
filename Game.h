#pragma once
#include <Gamebuino-Meta.h>
#include "Hexagon.h"
#include "Config.h"

class Game
{
public:
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
private:
    Hexagon hexagon;
};