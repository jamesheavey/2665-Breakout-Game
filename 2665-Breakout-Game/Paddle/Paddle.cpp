#include "Paddle.h"

FXOS8700CQ accelerometer(I2C_SDA,I2C_SCL);
    
Paddle::Paddle()
{

}

Paddle::~Paddle()
{

}

void Paddle::init(int y,int height,int width)
{
    _x = WIDTH/2 - width/2;  // initialised in the centre of the screen
    _y = y;  // fixed at the bottom of the screen, initialised by _paddley in engine
    _height = height; // paddle height, defined at init
    _width = width; // paddle width, defined at init
    _sens = 0.5; // default sensitivity of tilt/joystick
    _speed = 4;  // default speed = _speed * _sens
    _lives = 6;  // number of lives = number of LEDs
    _tilt = false;  //used to choose between motion methods

}


void Paddle::draw(N5110 &lcd)
{
    lcd.drawRect(_x,_y,_width,_height,FILL_BLACK);   // draw paddle at specified coordinates
}


void Paddle::update(Direction d,float mag)   // update the paddle's properties
{
    if (_tilt == false) {
        _speed = int(mag*10.0f);  // scale is arbitrary

        // update x value depending on direction of movement
        // West is decrement as origin is at the top-left so decreasing moves left
        if (d == W) {
            _x-=_speed * _sens;   // adjust the x coordinate accordingly. Sensitivity scales the speed of the paddle
        } else if (d == E) {
            _x+=_speed * _sens;
        }

        // censure that the paddle doesn't go off screen
        if (_x < 1) {
            _x = 1;
        }
        if (_x > WIDTH - _width - 1) {
            _x = WIDTH - _width - 1;
        }
    } else if (_tilt == true) {
        accelerometer.init();     // initialise the accelerometer
        Data values = accelerometer.get_values();     // retrieve a struct of values
        float roll_rad = atan2(values.ay,values.az);  // use values to calculate the roll angle in radians
        _speed = int((roll_rad*(360/3.14159265))*0.3f);  // convert to degrees and scale
        _x -= _speed * _sens;  // adjust the x coordinate accordingly
    }

    // ensure that the paddle doesn't go off screen
    if (_x < 1) {
        _x = 1;
    }
    if (_x > WIDTH - _width - 1) {
        _x = WIDTH - _width - 1;
    }
}


int Paddle::get_lives()  // returns the integer _lives
{
    return _lives;
}


void Paddle::lose_life()  // decrements the member variable _lives
{
    _lives--;
}


void Paddle::inc_life()  // increment the member variable _lives
{
    _lives++;
}


void Paddle::reset_lives()
{
    _lives = 6;
}


Vector2D Paddle::get_pos()  // returns the vector of the paddle's current position
{
    Vector2D p = {_x,_y};
    return p;
}


void Paddle::set_tilt()    // sets the paddle motion option to tilt i.e. the member variable _tilt = true
{
    _tilt = true;
}


void Paddle::set_joy()     // sets the paddle motion option to joystick
{
    _tilt = false;
}


void Paddle::recentre()    // moves the paddle back to the centre of the screen (used after victory screen to restart)
{
    _x = WIDTH/2 - PADDLE_WIDTH/2;  // centre of the screen
}


void Paddle::set_sens(float sens)  // sets the member variable _sens to an input
{
    _sens = sens;
}
