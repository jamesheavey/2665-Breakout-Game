#include "Brick.h"

Brick::Brick()
{

}

Brick::~Brick()
{

}

void Brick::init(int x,int y, int lives) // initialise the Brick object at the specified x, y coordinate and with the specified number of lives
{
    _x = x;    // x value on screen is fixed and must be defined at initialisation
    _y = y;    // y value on screen is fixed and must be defined at initialisation
    _height = BRICK_HEIGHT; // set brick height and width defined in Brick.h
    _width = BRICK_WIDTH;
    _lives = lives;     // lives set at initialisation, will be decremented upon collisions
    _base_lives = lives;    // _base_lives variable used to reset the bricks to the number of lives specified at init

}


void Brick::draw(N5110 &lcd) // draw the brick on the LCD
{
    if (_x >= 0) {    // only draw if on screen (more efficient?)
        if (_lives <= 2) {
            lcd.drawRect(_x,_y,_width,_height,FILL_TRANSPARENT);   // if lives < 3 then draw a hollow brick
        } else if (_lives <= 4) {
            lcd.drawRect(_x,_y,_width,_height,FILL_BLACK);
            lcd.drawRect(_x + 3, _y + 1,_width-6,_height-2,FILL_WHITE);   // if lives < 5 then fill up the brick a little bit (less hollow)
        } else {
            lcd.drawRect(_x,_y,_width,_height,FILL_BLACK);   // if lives >= 5 then draw a fully black brick
        }
    }
}


bool Brick::hit()  // decrement the number of lives by one (triggered by collisions), returns a bool
{
    _lives = _lives - 1; // decrement lives
    if (_lives <= 0) {
        return true;     // if lives <=0 brick is destroyed and will need to be moved off screen
    } else {
        return false;    // else the brick is still alive and can remain where it is
    }
}


void Brick::reset_lives(int inc)  // reset the lives after victory, takes inc
{
    _lives = _base_lives + inc;   // lives goes back to what it was at init plus 1 for every victory screen (inc = _multiplier in the breakout engine)
}

int Brick::get_x()   // return the brick's x coordinate (broken up into individual coordinates so it is easier to work with for list iterators)
{
    return _x;
}


int Brick::get_y()   // return the brick's y coordinate
{
    return _y;
}


void Brick::set_posx(int x) // set the brick's x coordinate (used to move it off and on screen). y is unneccessary
{
    _x = x;
}
