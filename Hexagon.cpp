#include "Hexagon.h"
#include "Patterns.h"
#include "Sound.h"
#include "Game.h"
#include "fx.h"

Hexagon::Hexagon(Game * game) : _game(game)
{
    _sound_channel = -1;
}

Hexagon::~Hexagon()
{

}

void Hexagon::init(const Hexagon::Level * level)
{
    if (_sound_channel != -1 || gb.sound.isPlaying(_sound_channel))
    {
        gb.sound.stop(_sound_channel);
    }

    _level = level;

    _sound_channel = gb.sound.play("level1.wav", true);

    _time = 0;
    _score = 0;
    _time_death = 0;
    _sides = 6;
    _state = State::PLAY;

    // Walls
    _wall_speed = _level->wall_speed;
    // init walls
    for (int i = 0; i < MAX_WALLS; i++)
    {
        pushWall({UINT8_MAX, 0, 0});
    }

    // Player
    _player_angle = 0;
    _player_speed = _level->player_speed;

    _angle_offset = 0;
    _angle_offset_speed = 16;
    _pump_offset = 0;
    _prev_wav_position = 0;

    _beat_sample_count = SOUND_FREQ * 60 / 100; 

    _high_score = 0;

    colorCallback = level->colorCallback;

    _pattern_spacing = 30;


    // Init pattern playing
    initPattern(Patterns::getRandomPattern(*_level));
    current_pattern_position=Utils::toFix(_pattern_spacing);
}

void Hexagon::initPattern(const Hexagon::Pattern * pattern)
{
    current_pattern = pattern;
    current_pattern_position = 0;
    current_pattern_wall_index = 0;
    current_pattern_lane_offset = rand() % _sides;
    current_pattern_length = 0;
    current_pattern_mirror = rand() % 2;
}

void Hexagon::update()
{
    // Keep track of time
    _time ++;
    computeAngleOffset();

    if (_time % 250 == 0)
    {
        _wall_speed += _level->wall_speed_incr;
        _player_speed += _level->player_speed_incr;
    }

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
    _score ++;

    // Update rotation
    if (_time % 127 == 0)
    {
        _angle_offset_speed = -(_angle_offset_speed * 4) / 3;
        if (abs(_angle_offset_speed) >= Utils::toFix(5))   // Cap the max speed
        {
            _angle_offset_speed = Utils::sign(_angle_offset_speed) * Utils::toFix(5);
        }
    }

    // Update pattern
    current_pattern_position += _wall_speed;
    for (int wall = current_pattern_wall_index; wall < current_pattern->nb_walls; wall ++)
    {
        if (current_pattern->walls[wall].distance + _pattern_spacing < Utils::fromFix(current_pattern_position))
        {
            current_pattern_wall_index++;
            Wall * new_wall = pushWall(current_pattern->walls[wall]);
            new_wall->distance = Utils::toFix(WALL_SPAWN_OFFSET);
            new_wall->lane = ( new_wall->lane + current_pattern_lane_offset) % _sides;

            if (current_pattern_mirror)
            {
                new_wall->lane = (_sides - new_wall->lane) % _sides;
            }

            // Save longest wall length
            current_pattern_length = max(current_pattern_length, current_pattern->walls[wall].distance + current_pattern->walls[wall].width);
        }
        else if (current_pattern_position > current_pattern_length + _pattern_spacing)
        {
            break;
        }
    }

    if (current_pattern_wall_index >= current_pattern->nb_walls )
    {
        initPattern(Patterns::getRandomPattern(*_level));
    }

    

    _center_distance = 6;

    // Pump
    _pump_offset -= PUMP_FALLOFF;

    if (_pump_offset < 0)
    {
        _pump_offset = 0;
    }

    //Gamebuino_Meta::Sound_Handler_Wav * wav_player = (Gamebuino_Meta::Sound_Handler_Wav *) (*gb.sound.getHandler(_sound_channel));

    if (_sound_channel != -1 && !gb.sound.isMute())
    {
        uint16_t _wav_position = gb.sound.getPos(_sound_channel)%_beat_sample_count;
        if (_wav_position < _prev_wav_position)
        {
            _pump_offset = PUMP_SET;
        }
        _prev_wav_position = _wav_position;
    }





    // Update all walls
    for (int i = 0; i < MAX_WALLS; i++)
    {
        if (isWallValid(_walls[i]))
        {
            _walls[i].distance -= _wall_speed;
            if (Utils::fromFix(_walls[i].distance) + (int16_t) _walls[i].width < 0)
            {
                _walls[i].lane = UINT8_MAX;
            } 
        }
    }

    // Move player
    _player_distance = _center_distance + PLAYER_OFFSET;
    int32_t new_player_angle = _player_angle;
    if (gb.buttons.repeat(BUTTON_LEFT,0))
    {
        new_player_angle -= _player_speed;
    }

    if (gb.buttons.repeat(BUTTON_RIGHT,0))
    {
        new_player_angle += _player_speed;
    }

    new_player_angle = Utils::smod(new_player_angle, Utils::toFix(256));

    // check for collision with the new position, then with the old one (so player don't loose if they hit the side of a wall)
    bool is_collision = false;
    for (int collision_pass = 0; collision_pass < 2; collision_pass++)
    {
        int pa = Utils::fromFix(new_player_angle);
        is_collision = false;
        for (int wall_index = 0; wall_index < MAX_WALLS; wall_index++)
        {
            Wall w = _walls[wall_index];
            int16_t d = Utils::fromFix(w.distance);
            if (isWallValid(w) && (w.lane == getLane(pa)) && d <= _player_distance && d + w.width >= _player_distance)
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

    if (gb.buttons.pressed(BUTTON_MENU))
    {
        _state = State::GAME_OVER;
    }

    _player_angle = new_player_angle;
}

void Hexagon::updateGameOver()
{
    if (_sound_channel != -1 || gb.sound.isPlaying(_sound_channel))
    {
        gb.sound.stop(_sound_channel);
    }

    if (_time_death == 0)
    {
        _game->gsfx.play(sfx_explosion);
        _high_score = _game->getScore(_level->id);
        is_highscore = _score > _high_score;
        if (is_highscore)
        {
            _game->setScore(_score, _level->id);
        }

    }

    _time_death ++;
    _angle_offset_speed = Utils::toFix(1) / 2;

    // zoom
    if (_time_death > DEATH_ZOOM_TIMING)
    {
        if (_pump_offset < PUMP_DEATH_MAX)
        {
            _pump_offset += PUMP_DEATH_ZOOM;
        }

        // Push wall off screen
        for (int i = 0; i < MAX_WALLS; i++)
        {
            if (isWallValid(_walls[i]))
            {
                _walls[i].distance = _walls[i].distance * 3 / 4;
                if ((Utils::toFix(_walls[i].distance)) > WALL_SPAWN_OFFSET)
                {
                    _walls[i].lane = UINT8_MAX;
                } 
            }
        }
    }

    // Do nothing at the moment
    if (gb.buttons.pressed(BUTTON_A))
    {
        init(_level);
    }
    if (gb.buttons.pressed(BUTTON_B))
    {
        _game->popState();
    }
}

void Hexagon::draw()
{
    // Compute values (sin,-cos) for the lanes
    Utils::computeLaneTrig(_angle_offset, _sides);

    // Init frame colors
    Color color_bg1,color_bg2,color_wall;
    (*colorCallback)(color_bg1,color_bg2,color_wall, _time);
    Color color_highlight = WHITE;

    // Clear to background color
    gb.display.clear(color_bg1);

    // Draw lanes color
    gb.display.setColor(color_bg2);
    for (int lane = 0; lane < _sides; lane += 2)
    {
        drawWall(lane, _center_distance + getPump(), WALL_SPAWN_OFFSET);
    }

    // Draw center hexagon
    gb.display.setColor(color_wall);

    // Draw all walls
    for (int i = 0; i < MAX_WALLS; i++)
    {
        if (isWallValid(_walls[i]))
        {
            // Avoid drawing at the center
            int distance = max(Utils::fromFix(_walls[i].distance), _center_distance);
            int width = _walls[i].width - (distance - (Utils::fromFix(_walls[i].distance)));
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
    int s = Utils::sin(Utils::fromFix(_player_angle) + Utils::fromFix(_angle_offset));
    int c = -Utils::cos(Utils::fromFix(_player_angle) + Utils::fromFix(_angle_offset));
    Utils::Point pos = {((s * (_player_distance + getPump())) >> 8) +  gb.display.width()/2,
                        ((c * (_player_distance + getPump())) >> 8) +  gb.display.height()/2};

    int offset = 1;
    if (_state == State::GAME_OVER)
    {
        if (_time_death < 6 && _time_death/2 % 2 == 0)
        {
            gb.display.clear(BLACK);
        }

        offset = (_time_death * _time_death) / 3;
        if ((_time_death / 4) % 2)
        {
            gb.display.setColor(color_highlight);
        }

        if (offset < 64)
        {
            Utils::Point pts[] = {{pos.x+offset, pos.y},{pos.x, pos.y+offset},{pos.x-offset, pos.y}, {pos.x, pos.y-offset}};

            for (int i = 0; i < 4; i++)
            {
                gb.display.drawLine(pts[i].x, pts[i].y,pts[(i+1)%4].x,pts[(i+1)%4].y);
            }
        }

    }
    else
    {
        gb.display.drawPixel(pos.x, pos.y);
        gb.display.drawPixel(pos.x+offset, pos.y);
        gb.display.drawPixel(pos.x-offset, pos.y);
        gb.display.drawPixel(pos.x, pos.y+offset);
        gb.display.drawPixel(pos.x, pos.y-offset);
    }

    // Display score

    if (_time_death < DEATH_ZOOM_TIMING)
    {
        gb.display.setColor(BLACK);
        for (int i = 0; i < 7; i++)
        {
            int x = 84 - 28 + i/2;
            gb.display.drawFastHLine(x, i, 28);
        }

        gb.display.setColor(WHITE);
        gb.display.setCursorX(84-24);
        gb.display.setCursorY(1);
        if ((_score / 25) < 100)
        {
            gb.display.printf("%02d:%02d", (_score / 25), ((_score % 25) * 100 / 25));
        }
        else
        {
            gb.display.printf("%03d:%01d", (_score / 25), ((_score % 25) * 10 / 25));
        }
    }
    else
    {
        gb.display.setColor(BLACK);
        for (int i = 0; i < 7; i++)
        {
            int x = 84 - 60 + i/2;
            gb.display.drawFastHLine(x, i + 23, 60);
            gb.display.drawFastHLine(x+4, i + 33, 60);
        }

        int x_offset = 10;
        int y_offset = 64-7;
        for (int i = 0; i < 7; i++)
        {
            gb.display.drawFastHLine(x_offset-i/2,y_offset+i, (40-(x_offset-i/2)) * 2);
        }

        gb.display.setColor(WHITE);
        gb.display.setCursorX(84-52);
        gb.display.setCursorY(24);
        gb.display.printf("LAST  %3d:%02d", (_score / 25), ((_score % 25) * 100 / 25));

        gb.display.setCursorX(84-48);
        gb.display.setCursorY(34);
        if (!is_highscore)
        {
            gb.display.printf("BEST %3d:%02d", (_high_score / 25), ((_high_score % 25) * 100 / 25));
        }
        else
        {
            gb.display.printf("NEW BEST!");
        }
        
        Utils::drawTextCenter(40, 64-6, "RETRY:\x15 QUIT:\x16");

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
        _lane_trig[i * 2]      = Utils::sin(i * (256/_sides) + Utils::fromFix(_angle_offset));
        _lane_trig[i * 2  + 1] = -Utils::cos(i * (256/_sides) + Utils::fromFix(_angle_offset));
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
        //SerialUSB.printf("[WARNING] Overriding existing wall\n");
    }
#endif

    _walls[_wall_counter] = wall;

    return &_walls[_wall_counter];
}

void Hexagon::computeAngleOffset()
{
    _angle_offset = Utils::smod(_angle_offset_speed + _angle_offset, Utils::toFix(256));
}