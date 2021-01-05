#ifndef BRICK_H
#define BRICK_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"

#define BRICK_WIDTH 12
#define BRICK_HEIGHT 4

/** Brick Class
@author James Heavey, University of Leeds
@brief Controls the bricks in the Breakout game
@date May 2019
*/

class Brick
{
public:

    /** Constructor declaration */
    Brick();
    
    /** Destructor declaration */
    ~Brick();
    
    /** Initialise Brick attributes
    * @param x @details initialises x coordinate
    * @param y @details initialises y coordinate
    * @param lives @details initialises the number of lives
    */
    void init(int x,int y, int lives);
    
    /** Draws the Brick on the LCD, at current coordinates with a fill that is dependant on _lives
    * @param lcd @details a N5110 pointer
    */
    void draw(N5110 &lcd);
    
    /** Sets the Brick's x coordinate
    * @param x @details set the  variable _x to the new local x
    */
    void set_posx(int x);
    
    /** Resets the lives after victory
    * @param inc @details additional lives added on t o the _base_lives (additional lives come from _multiplier)
    */
    void reset_lives(int inc);
    
    /** Decrements the  variable _lives
    * @returns a bool; true if brick is destroyed, false if not
    */
    bool hit();
    
    /** Retrieves the Brick's x coordinate
    * @returns integer _x coordinate
    */
    int get_x();
    
    /** Retrieves the Brick's y coordinate
    * @returns integer _y coordinate
    */
    int get_y();

private:

    int _height;
    int _width;
    int _x;
    int _y;
    int _lives;
    int _base_lives;

};
#endif