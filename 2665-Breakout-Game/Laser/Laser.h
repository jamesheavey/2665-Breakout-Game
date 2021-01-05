#ifndef LASER_H
#define LASER_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
#include "Paddle.h"

/** Laser Class
@author James Heavey, University of Leeds
@brief Controls the Laser in the Breakout game
@date May 2019
*/
class Laser
{

public:

    /** Constructor declaration */
    Laser();
    
    /** Deconstructor declaration */
    ~Laser();
    
    /** Initialises the laser off screen with a set velocity */
    void init();
    
    /** Draws the Laser at at its current coordinates on the LCD
    * @param lcd @details a N5110 pointer
    */
    void draw(N5110 &lcd);
    
    /** Update the Laser's y coordinate based on its velocity */
    void update();
    
    /** Sets the Laser's x coordinate
    * @param x @details set the  variable _x to the new local x
    */
    void set_posx(int x);
    
    /** Sets the Laser's x coordinate
    * @param y @details set the  variable _y to the new local y
    */
    void set_posy(int y);
    
    /** Retrieve the Laser's current x coordinate
    * @returns variable _x as an integer
    */
    int get_x();
    
    /** Retrieve the Laser's current y coordinate
    * @returns variable _y as an integer
    */
    int get_y();

private:

    int _speed_y;
    int _x;
    int _y;
    
};
#endif