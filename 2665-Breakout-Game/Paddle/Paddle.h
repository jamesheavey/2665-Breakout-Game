#ifndef PADDLE_H
#define PADDLE_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"

#define PADDLE_WIDTH 15
#define PADDLE_HEIGHT 2

/** Paddle Class
@author James Heavey, University of Leeds
@brief Controls the paddle in the Breakout game
@date May 2019
*/

class Paddle
{
public:
    
    /** Constructor declaration */
    Paddle();
    
    /** Destructor declaration */
    ~Paddle();
    
    /** Initialise Paddle attributes
    * @param y @details initialises the _y value at the correct value on screen
    * @param height @details sets the height of the paddle
    * @param width @details sets the width of the paddle
    */
    void init(int y,int height,int width);
    
    /** Draws the Paddle at at its current coordinates on the LCD
    * @param lcd @details a N5110 pointer
    */
    void draw(N5110 &lcd);
    
    /** Update the Paddle's velocity according to the Gamepad input, then update coordinates accordingly
    * @param pad @details a Gamepad pointer
    */
    void update(Direction d,float mag);
    
    /** Decrement the variable _lives */
    void lose_life();
    
    /** Increment the variable _lives */
    void inc_life();
    
    /** Set the variable _lives to 6 */
    void reset_lives();
    
    /** Retrieves the Paddle's lives remaining 
    * @returns the variable _lives
    */
    int get_lives();
    
    /** Sets the varible _tilt to true */
    void set_tilt();
    
    /** Set the variable _tilt to false */
    void set_joy();
    
    /** Set the variable _sens 
    * @param sens @details sets _sens to local variable sens
    */
    void set_sens(float sens);
    
    /** Set the _x coordinate to the middle of the screen */
    void recentre();
    
    /** Retrieves the Paddle's x and y velocities 
    * @returns a vector of the x and y velocities
    */
    Vector2D get_pos();

private:

    int _height;
    int _width;
    int _x;
    int _y;
    int _speed;
    int _score;
    int _lives;
    float _sens;
    bool _tilt;

};
#endif