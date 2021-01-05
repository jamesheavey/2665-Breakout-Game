#include "Laser.h"

Laser::Laser()
{

}

Laser::~Laser()
{

}

void Laser::init()  // initialises the lasers off screen
{
    _x = -10;  
    _y = 0;

    _speed_y = -2; // -2 speed in the y direction, doesnt change even off screen
}

void Laser::draw(N5110 &lcd)
{
    if (_x >= 0) {  // only draw if on screen (more efficient?)
        lcd.drawRect(_x,_y,2,6,FILL_BLACK);
    }
}

void Laser::update() // updates the laser's y coordinate according to its speed
{
    _y += _speed_y;
}

int Laser::get_x()  // retrieves the lasers's x coordinate
{
    return _x;
}

int Laser::get_y()  // retrieves the lasers's y coordinate
{
    return _y;
}

void Laser::set_posx(int x)  // sets the laser's x coordinate
{
    _x = x;
}

void Laser::set_posy(int y)  // sets the lasers's y coordinate
{
    _y = y;
}