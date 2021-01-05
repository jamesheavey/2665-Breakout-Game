#ifndef LIFE_POWERUP_H
#define LIFE_POWERUP_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
#include "Paddle.h"
#include "Bitmap.h"

static int powerup_data[] = 
{
    0, 0, 0, 1, 1, 1, 0, 0, 0, 
    0, 1, 1, 0, 0, 0, 1, 1, 0, 
    0, 1, 0, 1, 0, 1, 0, 1, 0, 
    1, 0, 1, 1, 1, 1, 1, 0, 1, 
    1, 0, 1, 1, 1, 1, 1, 0, 1, 
    1, 0, 0, 1, 1, 1, 0, 0, 1, 
    0, 1, 0, 0, 1, 0, 0, 1, 0, 
    0, 1, 1, 0, 0, 0, 1, 1, 0, 
    0, 0, 0, 1, 1, 1, 0, 0, 0
};

/** Life_Powerup Class
@author James Heavey, University of Leeds
@brief Controls the Life_Powerup in the Breakout game
@date May 2019
*/

class Life_Powerup
{
public:

    /** Constructor declaration */
    Life_Powerup();
    
    /** Destructor declaration */
    ~Life_Powerup();
    
    /** Initialises the powerup off screen with a set velocity */
    void init();
    
    /** Draws the powerup at at its current coordinates on the LCD
    * @param lcd @details a N5110 pointer
    */
    void draw(N5110 &lcd);
    
    /** Update the powerup's y coordinate based on its velocity */
    void update();
    
    /** Sets the powerups's x coordinate
    * @param x @details set the variable _x to the new local x
    */
    void set_posx(int x);
    
    /** Sets the powerups's x coordinate
    * @param y @details set the variable _y to the new local y
    */
    void set_posy(int y);
    
    /** Retrieve the powerup's current x coordinate
    * @returns variable _x as an integer
    */
    int get_x();
    
    /** Retrieve the powerup's current y coordinate
    * @returns variable _y as an integer
    */
    int get_y();

private:
    int _speed_y;
    int _x;
    int _y;
};
#endif