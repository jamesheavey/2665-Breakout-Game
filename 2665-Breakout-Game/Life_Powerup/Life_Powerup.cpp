#include "Life_Powerup.h"

Life_Powerup::Life_Powerup()
{

}

Life_Powerup::~Life_Powerup()
{

}

void Life_Powerup::init()  // initialises the Life_Powerup off screen
{
    _x = -10;  
    _y = 0;
    _speed_y = 1; // -2 speed in the y direction, doesnt change even off screen
}


void Life_Powerup::draw(N5110 &lcd)
{   
    if (_x >= 0) {  // only draw if on screen (more efficient?)
        Bitmap powerup(powerup_data, 9, 9);  // assign the powerup sprite data
        powerup.render(lcd,_x,_y);
    }
}


void Life_Powerup::update() // updates the Life_Powerup's y coordinate according to its speed
{
    _y += _speed_y;
}


int Life_Powerup::get_x()  // retrieves the Life_Powerups's x coordinate
{
    return _x;
}


int Life_Powerup::get_y()  // retrieves the Life_Powerups's y coordinate
{
    return _y;
}


void Life_Powerup::set_posx(int x)  // sets the Life_Powerup's x coordinate
{
    _x = x;
}


void Life_Powerup::set_posy(int y)  // sets the Life_Powerups's y coordinate
{
    _y = y;
}