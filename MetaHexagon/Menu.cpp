#include "Menu.h"
#include <Gamebuino-Meta.h>
#include "Game.h"
#include "Utils.h"
#include "Patterns.h"
#include "fx.h"


#define ALPHA

#ifdef ALPHA
const char * Menu::_str_unlock_help = "ALPHA VERSION ONLY ONE LEVEL AVAILABLE SORRY";
#else
const char * Menu::_str_unlock_help = "UNLOCK BY SURVIVING 60:00S IN LEVEL #";
#endif

Menu::Menu(Game * game) : _game(game)
{

}

Menu::~Menu()
{

}

void Menu::init()
{
    _level = 0;
    _time = 0;
    _angle = Utils::toFix(256/12);
    _scroll = 0;
}

void Menu::update()
{

    _time ++;
    _scroll ++;
    if (gb.buttons.pressed(BUTTON_LEFT))
    {
        _level = Utils::smod(_level - 1, Patterns::LEVEL_COUNT);
        _scroll = 0;
        _game->gsfx.play(sfx_ok,2);
    }
    if (gb.buttons.pressed(BUTTON_RIGHT))
    {
        _level = Utils::smod(_level + 1, Patterns::LEVEL_COUNT);
        _scroll = 0;
        _game->gsfx.play(sfx_ok,2);
    }

    uint32_t score = _game->getScore(_level);
    bool is_unlocked = _level < 3 || _game->getScore(_level-3) >= 25*60;
    #ifdef ALPHA
        is_unlocked = _level == 0;
    #endif

    if (gb.buttons.pressed(BUTTON_A))
    {
        if (is_unlocked)
        {
            _game->startHexagon(_level);
            _game->gsfx.play(sfx_ok,2);
        }
        else
        {
            _game->gsfx.play(sfx_err,2);
        }
    }

    if (gb.buttons.pressed(BUTTON_B))
    {
        _game->popState();
        _game->gsfx.play(sfx_back,2);
    }


    int32_t target = Utils::toFix(256/12 - (_level) * 256/6);
    int32_t delta = (target - _angle);
    
    // Avoid hexagon making a huge spin
    if (abs(delta) > Utils::toFix(127))
    {
        _angle += Utils::sign(delta) * Utils::toFix(256);
        delta = (target - _angle);
    }

    if (abs(delta) > 16)
    {
        _angle += delta >> 2;
    }
    else
    {
        _angle = target;
    }
}

void Menu::draw()
{
    uint32_t score = _game->getScore(_level);
    bool is_unlocked = _level < 3 || _game->getScore(_level-3) >= 25*60;
    #ifdef ALPHA
        is_unlocked = _level == 0;
    #endif

    Utils::computeLaneTrig(_angle, 6);
    Color bg1, bg2, walls;
    if (is_unlocked)
    {
        Patterns::levels[_level]->colorCallback(bg1, bg2, walls, _time);
    }
    else
    {
        Patterns::colorCallback_lock(bg1, bg2, walls, _time);
    }
    gb.display.clear(bg1);


    gb.display.setColor(bg2);


    
    for (int i = 0; i < 6; i+=2)
    {
        Utils::Point pt1 = Utils::getPoint(i, 10, 6);
        Utils::Point pt2 = Utils::getPoint(i, 100, 6);
        Utils::Point pt3 = Utils::getPoint((i+1)%6, 10, 6);
        Utils::Point pt4 = Utils::getPoint((i+1)%6, 100, 6);
        pt1.y += 18;
        pt2.y += 18;
        pt3.y += 18;
        pt4.y += 18;
        drawQuad(pt1, pt2, pt3, pt4);
    }

    gb.display.setColor(walls);

    Utils::Point pt1 = Utils::getPoint(0, 10, 6);
    for (int i = 0; i < 6; i++)
    {
        Utils::Point pt2 = Utils::getPoint((i+1)%6, 10, 6);
        gb.display.drawLine(pt1.x, pt1.y + 18, pt2.x, pt2.y+18);
        pt1 = pt2;
    }

    // Draw player
    gb.display.drawPixel(40     ,36);
    gb.display.drawPixel(40+1   ,36);
    gb.display.drawPixel(40-1   ,36);
    gb.display.drawPixel(40     ,36+1);
    gb.display.drawPixel(40     ,36-1);

    //gb.display.printf("This is menu\nLevel %d\nPress A\n", _level);
    
    // META HEXAGON BAR
    gb.display.setColor(BLACK);
    int x_offset = 10;
    int y_offset = 0;
    for (int i = 0; i < 7; i++)
    {
        gb.display.drawFastHLine(x_offset+i/2,y_offset+i, (40-(x_offset+i/2)) * 2);
    }

    gb.display.setColor(WHITE);
    Utils::drawTextCenter(40,y_offset+1,"META HEXAGON");

    gb.display.setColor(BLACK);
    
    // LEVEL BAR
    x_offset = 20;
    y_offset = 8;
    for (int i = 0; i < 7; i++)
    {
        gb.display.drawFastHLine(x_offset+i/2,y_offset+i, (40-(x_offset+i/2)) * 2);
    }

    // SCORE BAR
    x_offset = 25;
    y_offset = 16;
    for (int i = 0; i < 7; i++)
    {
        gb.display.drawFastHLine(x_offset+i/2,y_offset+i, (40-(x_offset+i/2)) * 2);
    }

    gb.display.setColor(WHITE);
    char buf[20];
    if (is_unlocked)
    {
        sprintf(buf, "LEVEL %d", _level+1);    
    }
    else
    {
        sprintf(buf, "LOCKED");
    }
    Utils::drawTextCenter(40,8+1,buf);
    sprintf(buf, "%3d:%02d", (score / 25), ((score % 25) * 100 / 25));
    Utils::drawTextCenter(40, 16+1, buf);

    // PRESS A TO PLAY


    if (is_unlocked)
    {
        gb.display.setColor(BLACK);
        x_offset = 20;
        y_offset = 64-7;
        for (int i = 0; i < 7; i++)
        {
            gb.display.drawFastHLine(x_offset-i/2,y_offset+i, (40-(x_offset-i/2)) * 2);
        }
        gb.display.setColor(WHITE);
        Utils::drawTextCenter(40, 64-6, "START:\x15");
    }
    else
    {
        gb.display.setColor(BLACK);
        x_offset = 0;
        y_offset = 64-7;
        for (int i = 0; i < 7; i++)
        {
            gb.display.drawFastHLine(x_offset-i/2,y_offset+i, (40-(x_offset-i/2)) * 2);
        }
        gb.display.setColor(WHITE);

        int offset = 0;
        if (_scroll > 25)
        {
            offset = (_scroll - 25) >> 2;
            offset = min(offset,strlen(_str_unlock_help)-19);
        }
        memcpy(buf, &_str_unlock_help[offset], 19);
        buf[19] = '\0';

        if (offset == strlen(_str_unlock_help)-19 )
        {
            #ifndef ALPHA
            buf[18] = '0' + _level-2;
            #endif
        }
        gb.display.setCursorX(1);
        gb.display.setCursorY(64-6);
        gb.display.printf(buf);
    }


    gb.display.setColor(walls);
    y_offset = 32;
    x_offset = 5;
    if (_time / 16 % 2)
    {
        x_offset --;
    }

    gb.display.drawLine(x_offset, y_offset, x_offset+3, y_offset-3);
    gb.display.drawLine(x_offset, y_offset, x_offset+3, y_offset+3);
    gb.display.drawLine(80-x_offset, y_offset, 80-(x_offset+3), y_offset-3);
    gb.display.drawLine(80-x_offset, y_offset, 80-(x_offset+3), y_offset+3);
}