#include <Gamebuino-Meta.h>
#include "Game.h"

Game game;

void setup() {
  // put your setup code here, to run once:
  gb.begin();
  game.init();
}

void loop() {
  // put your main code here, to run repeatedly:
    if (gb.update())
    {
        game.update();
    }
}
