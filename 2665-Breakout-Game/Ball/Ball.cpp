#include "Ball.h"

Ball::Ball()
{

}

Ball::~Ball()
{

}

void Ball::init(int size,int speed, int x)
{
    _size = size; // size of the ball

    _x = x;   // init position of the ball will be at the centre of the paddle
    _y = HEIGHT - _size/2 - 3; // fixed y init position, just above the paddle

    srand(time(NULL));
    int direction = rand() % 2; // randomise initial direction.

    // 2 directions, both directed upwards (N) from the paddle
    if (direction == 0) {
        _velocity.x = speed; // direction = E
        _velocity.y = -speed; // direction = N
    } else {
        _velocity.x = -speed; // direction = W
        _velocity.y = -speed; // direction = N
    }
}


void Ball::draw(N5110 &lcd)  // draw the ball at the specified location
{
    lcd.drawRect(_x,_y,_size,_size,FILL_BLACK);
}


void Ball::update()  // update the ball's position, based on its current velocity
{
    _x += _velocity.x;
    _y += _velocity.y;
}


void Ball::set_velocity(Vector2D v) // set the velocity in its x/y directions (used for collision velocity correction)
{
    _velocity.x = v.x;
    _velocity.y = v.y;
}


Vector2D Ball::get_velocity() // retrieve the ball's current velocities
{
    Vector2D v = {_velocity.x,_velocity.y};
    return v;
}


Vector2D Ball::get_pos() // retrieve the ball's current coordinates
{
    Vector2D p = {_x,_y};
    return p;
}


void Ball::set_pos(Vector2D p)  // set the ball's coordinates (used for collision velocity correction)
{
    _x = p.x;
    _y = p.y;
}