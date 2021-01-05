#ifndef BREAKOUTENGINE_H
#define BREAKOUTENGINE_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "Laser.h"
#include "Life_Powerup.h"

#include <list>
#include <iterator>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

#define GAP_TOP 10
#define GAP 2

/** BreakoutEngine Class
@author James Heavey, University of Leeds
@brief Controls the Breakout game
@date May 2019
*/

class BreakoutEngine
{

public:

    /** Constructor declaration */
    BreakoutEngine();
    
    /** Destructor declaration */
    ~BreakoutEngine();
    
    /** Initialises game variables
    * @param paddle_width @details integer width of paddle
    * @param paddle_height @details integer height of paddle
    * @param ball_size @details integer diameter of ball in pixels
    * @param speed @details integer initial speed of ball
    */
    void init(int paddle_width,int paddle_height,int ball_size,int speed);
    
    /** Reads inputs from the Gamepad
    * @param pad @details a Gamepad pointer
    */
    void read_input(Gamepad &pad);
    
    /** Updates the Game attributes
    * @param pad @details a Gamepad pointer
    */
    void update(Gamepad &pad);
    
    /** Draws the objects' at their respective coordinates on the LCD
    * @param lcd @details a N5110 pointer
    */
    void draw(N5110 &lcd);
    
    /** Updates the Gamepad LEDs with lives remaining
    * @param pad @details a Gamepad pointer
    */
    void lives_leds(Gamepad &pad);
    
    /** Sets the variable _prev_score to current score
    * @param prev_score @details the new previous score which is the score achieved at victory
    */
    void set_prev_score(int prev_score);
    
    /** Increments the laser index */
    void inc_index();
    
    /** Resets the laser index to 0 */
    void reset_index();
    
    /** Returns the game (and all relevant objects) to initialised state  */
    void reset_game();
    
    /** Sets the variable _number_left to 18 */
    void reset_num_left();
    
    /** Increment the multiplier if continue is selected upon victory */
    void inc_mult();
    
    /** Resets the multiplier to 0
    * @returns _prev_score
    */
    void reset_mult();
    
    /** Sets the paddle motion options for use in game
    * @param tilt @details a bool that sets tilt if true and joystick if false
    * @param sens @details a float that is derived from the pot, it multiplies the velocity of the paddle
    */
    void set_paddle_motion(bool tilt, float sens);
    
    /** Sets the powerup to a random x position on screen if conditions are met */
    void check_life_powerup();
    
    /** Resets the paddle lives to 6, used to reset the game  */
    void reset_paddle_lives();
    
    /** Decrements the number of bricks left on screen when one is destroyed  */
    void dec_num_left();
    
    /** Returns the number of Bricks remaining on screen
    * @returns _number_left
    */
    int get_num_left();
    
    /** Returns the score achieved on victory in the previous iteration of the game
    * @returns _prev_score
    */
    int get_prev_score();
    
    /** Returns the number of lives remaining 
    * @returns the paddle variable _lives
    */
    int get_paddle_lives();
    
    /** Returns the current multiplier value
    * @returns _multiplier
    */
    int get_mult();
    
    /** Checks if the ball goes past the screen y boundary
    * @param pad @details a Gamepad pointer
    * @returns a bool; true if ball is off screen, false
    */
    bool check_loss(Gamepad &pad);
    
    /** Returns the current score to print to LCD in the game
    * @returns _score
    */
    int get_score();

private:

    void check_wall_collisions(Gamepad &pad);  
    void check_paddle_collisions(Gamepad &pad);
    void check_brick_collisions(Gamepad &pad);
    void check_laser_collisions(Gamepad &pad);
    void check_powerup_collisions(Gamepad &pad);
    void print_scores(N5110 &lcd);

    int _paddle_width;
    int _paddle_height;
    int _ball_size;
    int _speed;
    int _index;
    int _multiplier;
    int _paddley;
    int _number_left;
    int _prev_score;
    int _score;
    double _cool_time;
    
    Direction _d;
    float _mag;

    std::list<Laser> listofLasers;
    std::list<Laser>::iterator it_L;

    std::list<Brick> listofBricks;
    std::list<Brick>::iterator it_R;
    
    Paddle _paddle;

    Ball _ball;

    Brick _brick11;
    Brick _brick12;
    Brick _brick13;
    Brick _brick14;
    Brick _brick15;
    Brick _brick16;
    Brick _brick21;
    Brick _brick22;
    Brick _brick23;
    Brick _brick24;
    Brick _brick25;
    Brick _brick26;
    Brick _brick31;
    Brick _brick32;
    Brick _brick33;
    Brick _brick34;
    Brick _brick35;
    Brick _brick36;

    Laser _laser1;
    Laser _laser2;
    Laser _laser3;

    Life_Powerup _powerup;

};

#endif