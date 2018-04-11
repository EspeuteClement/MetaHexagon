#include "Game.h"
#include "Patterns.h"


Game::Game() : _menu(this), hexagon(this), _title(this)
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
    _state_ptr = 0;
    _menu.init();
    _title.init();
    gb.sound.playOK();
    gsfx.init();
    pushState(State::TITLE);

    // init save
    for (int i = 0; i < 6; i++)
    {
        _scores[i] = gb.save.get(i);
    }
}

void Game::update()
{
    // Update the game !
    switch (_state_stack[_state_ptr])
    {
        case State::TITLE:
            _title.update();
            break;
        case State::MENU:
            _menu.update();
            break;
        case State::HEXAGON:
            hexagon.update();
            break;
        default:
            abort();
            break;
    }
}

void Game::draw()
{
    // Draw the game!
    switch (_state_stack[_state_ptr])
    {
        case State::TITLE:
            _title.draw();
            break;
        case State::MENU:
            _menu.draw();
            break;
        case State::HEXAGON:
            hexagon.draw();
            break;
        default:
            abort();
            break;
    }
}

void Game::setScore(uint32_t score, uint8_t index)
{
    _scores[index] = score;
    gb.save.set(index, score);
}

uint32_t Game::getScore(uint8_t index)
{
    return _scores[index];
}

void Game::startHexagon(uint8_t level_id)
{
    hexagon.init(Patterns::levels[level_id]);
    pushState(State::HEXAGON);
}

void Game::startMenu()
{
    _menu.init();
    pushState(State::MENU);
}