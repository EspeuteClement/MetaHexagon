#include <Gamebuino-Meta.h>
#include "Game.h"

// Control whenever we should use debug information or not


Game game;

void setup() {
  // put your setup code here, to run once:

  gb.begin();
  game.init();

#ifdef __DEBUG_OUTPUT__
  SerialUSB.begin(9600);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
    if (gb.update())
    {
        game.update();
        game.draw();
    }
}
