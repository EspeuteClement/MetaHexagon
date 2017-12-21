#include "Hexagon.h"
#include "Patterns.h"
#include "Sound.h"

Hexagon::Hexagon()
{
    _sound_channel = -1;
}

Hexagon::~Hexagon()
{

}

void Hexagon::init()
{
    if (_sound_channel != -1 || gb.sound.isPlaying(_sound_channel))
    {
        gb.sound.stop(_sound_channel);
    }

    //_sound_channel = gb.sound.play(new Sound_Handler_Custom(NULL), true);


    _time = 0;
    _sides = 6;
    _state = State::PLAY;

    // Walls
    _wall_speed = 2;
    // init walls
    for (int i = 0; i < MAX_WALLS; i++)
    {
        pushWall({UINT8_MAX, 0, 0});
    }

    // Player
    _player_angle = 0;
    _player_speed = 8;

    // Init pattern playing
    initPattern(Patterns::getRandomPattern());
}

void Hexagon::initPattern(const Hexagon::Pattern * pattern)
{
    current_pattern = pattern;
    current_pattern_position = 0;
    current_pattern_wall_index = 0;
    current_pattern_lane_offset = rand() % _sides;
}

void Hexagon::update()
{
    // Keep track of time
    _time ++;
    computeAngleOffset();

    switch (_state)
    {
        case State::PLAY :
            updatePlay();
            break;
        case State::GAME_OVER :
            updateGameOver();
            break;
        default:
            break;
    }
}

void Hexagon::updatePlay()
{
    // Push a random wall in the field
    if (gb.buttons.pressed(BUTTON_A))
    {
        pushWall({rand()%_sides, 60, rand()%10 + 4 });
    }

    // Update pattern
    current_pattern_position += _wall_speed;
    for (int wall = current_pattern_wall_index; wall < current_pattern->nb_walls; wall ++)
    {
        if (current_pattern->walls[wall].distance < current_pattern_position)
        {
            current_pattern_wall_index++;
            Wall * new_wall = pushWall(current_pattern->walls[wall]);
            new_wall->distance = WALL_SPAWN_OFFSET;
            new_wall->lane = ( new_wall->lane + current_pattern_lane_offset) % _sides;
        }
        else
        {
            break;
        }
    }

    if (current_pattern_wall_index >= current_pattern->nb_walls )
    {
        initPattern(Patterns::getRandomPattern());
    }

    

    _center_distance = 6;

    // Pump
    if (_time % PUMP_FREQ == 0)
    {
        _pump_offset = PUMP_SET;
    }
    else
    {
        _pump_offset -= PUMP_FALLOFF;
    }

    if (_pump_offset < 0)
    {
        _pump_offset = 0;
    }



    // Update all walls
    for (int i = 0; i < MAX_WALLS; i++)
    {
        if (isWallValid(_walls[i]))
        {
            _walls[i].distance -= _wall_speed;
            if (_walls[i].distance + (int16_t) _walls[i].width < 0)
            {
                _walls[i].lane = UINT8_MAX;
            } 
        }
    }

    // Move player
    _player_distance = _center_distance + PLAYER_OFFSET;
    int new_player_angle = _player_angle;
    if (gb.buttons.repeat(BUTTON_LEFT,0))
    {
        new_player_angle -= _player_speed;
    }

    if (gb.buttons.repeat(BUTTON_RIGHT,0))
    {
        new_player_angle += _player_speed;
    }

    new_player_angle = (new_player_angle % 256 + 256) % 256;

    // check for collision with the new position, then with the old one (so player don't loose if they hit the side of a wall)
    bool is_collision = false;
    for (int collision_pass = 0; collision_pass < 2; collision_pass++)
    {
        is_collision = false;
        for (int wall_index = 0; wall_index < MAX_WALLS; wall_index++)
        {
            Wall w = _walls[wall_index];
            if (isWallValid(w) && (w.lane == getLane(new_player_angle)) && w.distance <= _player_distance && w.distance + w.width >= _player_distance)
            {
#ifdef __DEBUG_OUTPUT__
                SerialUSB.printf("[LOG] HIT WALL\n");
#endif
                is_collision = true;
                break;
            }
        }


        if (is_collision)
        {
            new_player_angle = _player_angle;
        }
        else
        {
            // No collision, we can go there
            break;
        }
    }

    if (is_collision)
    {
        _state = State::GAME_OVER;
        return;
    }

    _player_angle = new_player_angle;
}

void Hexagon::updateGameOver()
{
    // Do nothing at the moment
    if (gb.buttons.pressed(BUTTON_A))
    {
        init();
    }
}

void Hexagon::draw()
{
    // Compute values (sin,-cos) for the lanes
    computeLaneTrig();

    // Init frame colors
    Color color_bg1   = gb.createColor(18, 78, 137);
    Color color_bg2   = gb.createColor(0,149,233);
    Color color_wall = gb.createColor(44,232,245);

    // Clear to background color
    gb.display.clear(color_bg1);

    // Draw lanes color
    gb.display.setColor(color_bg2);
    for (int lane = 0; lane < _sides; lane += 2)
    {
        drawWall(lane, _center_distance + getPump(), 60);
    }

    // Draw center hexagon
    gb.display.setColor(color_wall);

    // Draw all walls
    for (int i = 0; i < MAX_WALLS; i++)
    {
        if (isWallValid(_walls[i]))
        {
            // Avoid drawing at the center
            int distance = max(_walls[i].distance, _center_distance);
            int width = _walls[i].width - (distance - _walls[i].distance);
            if (width > 0)
            {
                drawWall(_walls[i].lane, distance + getPump(), width);
            }

        }
    }

    for (int lane = 0; lane < _sides; lane ++)
    {
        Utils::Point p1 = getPoint(lane, _center_distance + getPump());
        Utils::Point p2 = getPoint(lane+1, _center_distance + getPump());
        gb.display.drawLine(p1.x, p1.y, p2.x, p2.y);
    }

    // draw the player
    int s = Utils::sin(_player_angle + _angle_offset);
    int c = -Utils::cos(_player_angle + _angle_offset);
    Utils::Point pos = {((s * (_player_distance + getPump())) >> 8) +  gb.display.width()/2,
                        ((c * (_player_distance + getPump())) >> 8) +  gb.display.height()/2};

    gb.display.drawPixel(pos.x, pos.y);
    gb.display.drawPixel(pos.x+1, pos.y);
    gb.display.drawPixel(pos.x-1, pos.y);
    gb.display.drawPixel(pos.x, pos.y+1);
    gb.display.drawPixel(pos.x, pos.y-1);

    // Display score
    gb.display.setColor(BLACK);
    for (int i = 0; i < 7; i++)
    {
        int x = 84 - 28 + i/2;
        gb.display.drawFastHLine(x, i, 28);
    }

    gb.display.setColor(WHITE);
    gb.display.setCursorX(84-24);
    gb.display.setCursorY(1);
    if ((_time / 25) < 100)
    {
        gb.display.printf("%02d:%02d", (_time / 25), ((_time % 25) * 100 / 25));
    }
    else
    {
        gb.display.printf("%03d:%01d", (_time / 25), ((_time % 25) * 10 / 25));
    }
    

#ifdef __DEBUG_OUTPUT__
    if (_time%25 == 0)
    {
        SerialUSB.printf("%d\n", gb.getCpuLoad());
    }
#endif
}


void Hexagon::computeLaneTrig()
{
    for (int i = 0; i < _sides; i++)
    {
        _lane_trig[i * 2]      = Utils::sin(i * 43 + _angle_offset);
        _lane_trig[i * 2  + 1] = -Utils::cos(i * 43 + _angle_offset);
    }
}

void Hexagon::benchmark()
{
    for (int i = 0; i < 6; i++)
    {
        _lane_trig[i * 2]      = Utils::sin(i * 43 + _time*4);
        _lane_trig[i * 2  + 1] = -Utils::cos(i * 43 + _time*4);
    }

    // Draw hexagon
    for (int i = 0; i < 6; i++)
    {
        const int distance = 6;
        int x1 = ((_lane_trig[i * 2] * distance) >> 8)  + 42;
        int y1 = ((_lane_trig[i * 2 + 1] * distance) >> 8) + 32;
        int x2 = ((_lane_trig[((i+1)%6) * 2] * distance) >> 8) + 42;
        int y2 = ((_lane_trig[((i+1)%6) * 2 + 1] * distance) >> 8) + 32;
        gb.display.drawLine(x1, y1 , x2, y2);
    }

    // Draw walls
    for (int d = 0; d < 6; d++)
    {
        int d1 = d * 10 + 10 ;
        int d2 = d1 + 5;
        for (int i = 0; i < 6; i++)
        {
            
            int x1 = ((_lane_trig[i * 2] * d1) >> 8)  + 42;
            int y1 = ((_lane_trig[i * 2 + 1] * d1) >> 8) + 32;
            int x2 = ((_lane_trig[((i+1)%6) * 2] * d1) >> 8) + 42;
            int y2 = ((_lane_trig[((i+1)%6) * 2 + 1] * d1) >> 8) + 32;

            int x3 = ((_lane_trig[i * 2] * d2) >> 8)  + 42;
            int y3 = ((_lane_trig[i * 2 + 1] * d2) >> 8) + 32;
            int x4 = ((_lane_trig[((i+1)%6) * 2] * d2) >> 8) + 42;
            int y4 = ((_lane_trig[((i+1)%6) * 2 + 1] * d2) >> 8) + 32;

            Utils::drawQuad({x1,y1},{x2,y2},{x3,y3},{x4,y4});
        }
    }
}

void Hexagon::drawWall(uint8_t lane, int16_t distance, int8_t width)
{
    Utils::Point p1 = getPoint(lane, distance);
    Utils::Point p2 = getPoint(lane, distance + width);
    Utils::Point p3 = getPoint(lane + 1, distance);
    Utils::Point p4 = getPoint(lane + 1, distance + width);

    Utils::drawQuad(p1, p2, p3, p4);
}

Hexagon::Wall * Hexagon::pushWall(const Wall & wall)
{
    _wall_counter = (_wall_counter + 1) % MAX_WALLS;

#ifdef __DEBUG_OUTPUT__
    if (isWallValid(_walls[_wall_counter]))
    {
        SerialUSB.printf("[WARNING] Overriding existing wall\n");
    }
#endif

    _walls[_wall_counter] = wall;

    return &_walls[_wall_counter];
}

void Hexagon::computeAngleOffset()
{
    _angle_offset = (_time*2)%256;
}