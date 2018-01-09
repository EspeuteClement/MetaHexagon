#pragma once
#include <cstdint>
#include <Gamebuino-Meta.h>
#include <utility/Sound-SD/Sound-SD.h>

#include "Utils.h"

class Game;

class Hexagon
{
public:
    // Constants
    static const uint8_t MAX_SIDES = 6;
    static const uint8_t MAX_WALLS = 32;
    static const uint8_t PLAYER_OFFSET = 3;
    static const uint8_t MAX_WALL_PER_PATTERN = 32;
    static const uint16_t WALL_SPAWN_OFFSET = 70; // At which distance a wall spawns
    static const uint8_t MAX_PATTERN_PER_LEVEL = 32;
    static const int32_t FPP = 5; // Fixed point arithmetic precision in bits
    
    static const uint16_t PUMP_SET = Utils::toFix(3); // How much the pump is at max level
    static const uint16_t PUMP_FREQ = 20; // How much frames we wait before we pump
    static const uint16_t PUMP_FALLOFF = 16; // divide by 256 
    
    static const uint32_t PUMP_DEATH_ZOOM = 32;
    static const uint32_t PUMP_DEATH_MAX = 20 << FPP;
    //static const uint32_t WALL_DEATH_SPEED = (1 << FPP) + 16;
    static const uint32_t DEATH_ZOOM_TIMING = 25;

    struct Wall
    {
        uint8_t lane; // If = UINT8_MAX, should not be drawn
        int32_t distance;
        uint8_t width;
    }; // 32 bit long struct

    struct Pattern
    {
        const uint8_t nb_walls;
        const Wall walls[MAX_WALL_PER_PATTERN]; 
    };

    struct Level
    {
        const uint8_t id;
        const int16_t player_speed;
        const int16_t player_speed_incr;
        const int16_t wall_speed;
        const int16_t wall_speed_incr;
        const Pattern * patterns[MAX_PATTERN_PER_LEVEL];
        const uint8_t nb_patterns;
        void (*colorCallback)(Color & bg1, Color & bg2, Color & walls, uint32_t time);
        int16_t (*angleSpeedCallback)(int16_t current_speed,uint32_t time);
    };

    Hexagon(Game * game);
    ~Hexagon();
    void init(const Level * level);

    void update();
    void updatePlay();
    void updateGameOver();
    
    void draw();



    enum class State
    {
        PLAY,
        PAUSE,
        GAME_OVER
    };

private:

    /** === ATTRIBUTES ===========================================================================*/
    // Music
    Game * _game;

    int8_t _sound_channel;

    uint32_t _time;      // Count the time inside the game
    uint32_t _score;     // Score/time of the player
    uint32_t _high_score;
    int32_t _lane_trig[MAX_SIDES * 2]; // Keep track of the lanes coeficients. i*2 = sin, i*2+1 = -cos
    uint8_t _sides;          // How much the hexagon has sides (so we can change to pentagon or square)

    const Level * _level;

    uint8_t _wall_speed;
    int16_t _pattern_spacing;
    Wall _walls[MAX_WALLS];

    const Pattern * current_pattern; // A pointer to the current pattern playing
    int16_t current_pattern_position; // Indicate how much distance we are in the pattern
    uint8_t current_pattern_wall_index; // Indicate the next wall to push
    uint8_t current_pattern_lane_offset; // Random lane offset for the current pattern
    int16_t current_pattern_length;
    bool current_pattern_mirror;

    void (*colorCallback)(Color & bg1, Color & bg2, Color & walls, uint32_t time);

    uint8_t _wall_counter; // 'Point' to the last inserted wall in the _walls array; 
    uint16_t _center_distance;

    // Player
    int16_t _player_angle;
    int16_t _player_speed;
    int16_t _player_distance;

    int16_t _angle_offset;
    int16_t _angle_offset_speed;
    int16_t _pump_offset;
    uint16_t _prev_wav_position;
    uint16_t _beat_sample_count;

    int32_t _time_death;
    bool is_highscore;
    State _state;

    /** === METHODS ===============================================================================*/
    /** Compute and cache all the nessesary values in order 
    * to display the hexagonal graphics. The values are stored in
    * _lane_trig */
    void computeLaneTrig();

    /** Set the drawing angle offset value of the game (makes our hexagon rotate)*/
    void computeAngleOffset();

    /** Draw a wall on the given lane, with the given width*/
    void drawWall(uint8_t lane, int16_t distance, int8_t width);

    /** Draw tons of wall on the screen in order to benchmark the engine*/
    void benchmark();

    /** Returns a reference to the inserted wall */
    Wall * pushWall(const Wall & wall);

    void initPattern(const Hexagon::Pattern * pattern);

    inline int16_t getPump() {return _pump_offset >> FPP;};

    /** === INLINE METHODS =========================================================================*/
    /** Returns a point on a lane at the given distance.
    */
    inline Utils::Point getPoint(uint8_t lane, int16_t distance) __attribute__((optimize("-O3")))
    {
        // I tried to make this function as fast as possible.
        return 
        {
            ((Utils::getLaneTrig((lane%_sides) * 2) * distance) >> 8) + gb.display.width()/2, // x
            ((Utils::getLaneTrig((lane%_sides) * 2 + 1) * distance) >> 8) + gb.display.height()/2 // y
        };
    }

    // Check if a wall should be considered as part of the game
    inline bool isWallValid(const Wall & w) { return w.lane < _sides;}

    inline uint8_t getLane(uint8_t angle) { return angle / (256/_sides);}; 
};