#pragma once
#include <cstdint>
#include <Gamebuino-Meta.h>

#include "Utils.h"

class Hexagon
{
public:
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
        int16_t distance;
        uint8_t width;
    }; // 32 bit long struct

    enum class State
    {
        PLAY,
        PAUSE,
        GAME_OVER
    };

private:

    /** === ATTRIBUTES ===========================================================================*/
    static const uint8_t MAX_SIDES = 6;
    static const uint8_t MAX_WALLS = 32;
    static const uint8_t PLAYER_OFFSET = 3;

    uint32_t _time;      // Count the time inside the game
    int32_t _lane_trig[MAX_SIDES * 2]; // Keep track of the lanes coeficients. i*2 = sin, i*2+1 = -cos
    uint8_t _sides;          // How much the hexagon has sides (so we can change to pentagon or square)

    uint8_t _wall_speed;
    Wall _walls[MAX_WALLS];
    uint8_t _wall_counter; // 'Point' to the last inserted wall in the _walls array; 
    uint16_t _center_distance;

    // Player
    int16_t _player_angle;
    int16_t _player_speed;
    int16_t _player_distance;

    int16_t _angle_offset;

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

    void pushWall(const Wall & wall);

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
    inline bool isWallValid(const Wall & w) { return w.lane != UINT8_MAX;}

    inline uint8_t getLane(uint8_t angle) { return angle / (256/_sides);}; 
};