#include "Title.h"
#include "Game.h"
#include "Utils.h"
#include "Patterns.h"
#include <Gamebuino-Meta.h>
#include "fx.h"

Title::Title(Game * game) : _game(game)
{

}

void Title::init()
{
    _time = 0;
}

void Title::update()
{
    _time ++;

    if (gb.buttons.pressed(BUTTON_A))
    {
        _game->startMenu();
        _game->gsfx.play(sfx_ok,2);
    }
}

void Title::draw()
{
    Utils::computeLaneTrig(_time << 7, 6);


    Color bg1, bg2, walls;
    Patterns::colorCallback_lock(bg1, bg2, walls, _time);

    Utils::Point p1, center;
    p1 = Utils::getPoint(0, 150, 6);
    center = {40,63};
    for (int i = 0; i < 6; i++)
    {
        if (i%2)
        {
            gb.display.setColor(bg1);
        }
        else
        {
            gb.display.setColor(bg2);
        }
        Utils::Point p2 = Utils::getPoint((i+1)%6,150,6);
        gb.display.fillTriangle(center.x, center.y, p1.x, p1.y, p2.x, p2.y);
        p1 = p2;
    }

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
    x_offset = 17;
    y_offset = 8;
    for (int i = 0; i < 7; i++)
    {
        gb.display.drawFastHLine(x_offset+i/2,y_offset+i, (40-(x_offset+i/2)) * 2);
    }

    gb.display.setColor(WHITE);
    Utils::drawTextCenter(40,y_offset+1,"BY VALDEN");


    gb.display.setColor(BLACK);
    x_offset = 15;
    y_offset = 51;
    for (int i = 0; i < 15; i++)
    {
        gb.display.drawFastHLine(x_offset-i/2,y_offset+i, (40-(x_offset-i/2)) * 2);
    }

    gb.display.setColor(WHITE);
    Utils::drawTextCenter(40,y_offset+1,"ORIGINAL BY");
    Utils::drawTextCenter(40,y_offset+7,"@TERRYCAVANAGH");

    if (_time & 0b10000)
    {
        gb.display.setColor(BLACK);
        x_offset = 22;
        y_offset = 35;
        for (int i = 0; i < 7; i++)
        {
            gb.display.drawFastHLine(x_offset+i/2,y_offset+i, (40-(x_offset+i/2)) * 2);
        }
        gb.display.setColor(WHITE);
        Utils::drawTextCenter(40,y_offset+1,"PRESS \x15");
    }
}
