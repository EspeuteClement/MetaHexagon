#include "Game.h"


Game::Game()
{
    // Should init nothing, leave that to Game::init()
}

Game::~Game()
{
    // Shound't do much either
}

void Game::init()
{
    // Init all the game value (in order to relaunch the game for example)
    hexagon.init();
}

void Game::update()
{
    // Update the game !
    hexagon.update();
}

void Game::draw()
{
    // Draw the game!
    hexagon.draw();
}