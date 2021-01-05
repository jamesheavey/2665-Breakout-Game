#ifndef BALL_H
#define BALL_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
#include "Paddle.h"

#define BALL_SIZE 2
#define BALL_SPEED 2

/** Ball Class
@author James Heavey, University of Leeds
@brief Controls the ball in the Breakout game
@date May 2019
*/

class Ball
{ 

public:
    
    /** Constructor declaration */
    Ball();
    
    /** Destructor declaration */
    ~Ball();
    
    /** Initialise Ball attributes
    * @param size @details initialises _size
    * @param speed @details initialises _speed
    * @param x @details initialises the _x coordinate over the centre of the paddle
    */
    void init(int size,int speed,int x);
    
    /** Draws the Ball at at its current coordinates on the LCD
    * @param lcd @details a N5110 pointer
    */
    void draw(N5110 &lcd);
    
    /** Update the Laser's x and y coordinates based on its velocity */
    void update();

    /** Set the Ball's x and y velocities 
    * @param v @details a vector of the x and y velocities
    */
    void set_velocity(Vector2D v);
    
    /** Set the Ball's x and y coordinates
    * @param p @details a vector of the x and y velocities
    */
    void set_pos(Vector2D p); 
    
    /** Retrieves the Ball's x and y velocities 
    * @returns a vector of the x and y velocities
    */
    Vector2D get_velocity();
    
    /** Retrieves the Ball's x and y velocities 
    * @returns a vector of the x and y velocities
    */
    Vector2D get_pos();
    
    

private:

    Vector2D _velocity;
    int _size;
    int _x;
    int _y;
};
#endif