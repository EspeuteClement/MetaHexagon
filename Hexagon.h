#pragma once
#include <cstdint>
#include <Gamebuino-Meta.h>

#include "Utils.h"

class Hexagon
{
public:
    // Constants
    static const uint8_t MAX_SIDES = 6;
    static const uint8_t MAX_WALLS = 32;
    static const uint8_t PLAYER_OFFSET = 3;
    static const uint8_t MAX_WALL_PER_PATTERN = 32;
    static const uint16_t WALL_SPAWN_OFFSET = 70; // At which distance a wall spawns

    static const int32_t FPP = 5; // Fixed point arithmetic precision in bits
    
    static const uint16_t PUMP_SET = 3 << FPP; // How much the pump is at max level
    static const uint16_t PUMP_FREQ = 20; // How much frames we wait before we pump
    static const uint16_t PUMP_FALLOFF = 16; // divide by 256 
    
    static const uint32_t PUMP_DEATH_ZOOM = 32;
    static const uint32_t PUMP_DEATH_MAX = 20 << FPP;
    static const uint32_t WALL_DEATH_SPEED = (1 << FPP) + 16;
    static const uint32_t DEATH_ZOOM_TIMING = 25;

    Hexagon();
    ~Hexagon();
    void init();

    void update();
    void updatePlay();
    void updateGameOver();
    
    void draw();

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

    enum class State
    {
        PLAY,
        PAUSE,
        GAME_OVER
    };

private:

    /** === ATTRIBUTES ===========================================================================*/
    // Music
    int8_t _sound_channel;

    uint32_t _time;      // Count the time inside the game
    uint32_t _score;     // Score/time of the player
    uint32_t _best_score;
    int32_t _lane_trig[MAX_SIDES * 2]; // Keep track of the lanes coeficients. i*2 = sin, i*2+1 = -cos
    uint8_t _sides;          // How much the hexagon has sides (so we can change to pentagon or square)

    uint8_t _wall_speed;
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

    int32_t _time_death;

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
            ((_lane_trig[(lane%_sides) * 2] * distance) >> 8) + gb.display.width()/2, // x
            ((_lane_trig[(lane%_sides) * 2 + 1] * distance) >> 8) + gb.display.height()/2 // y
        };
    }

    // Check if a wall should be considered as part of the game
    inline bool isWallValid(const Wall & w) { return w.lane < _sides;}

    inline uint8_t getLane(uint8_t angle) { return angle / (256/_sides);}; 
};