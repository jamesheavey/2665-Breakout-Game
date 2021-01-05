#include "BreakoutEngine.h"

BreakoutEngine::BreakoutEngine()
{

}

BreakoutEngine::~BreakoutEngine()
{

}

void BreakoutEngine::init(int paddle_width,int paddle_height,int ball_size,int speed)
{
    // initialise the game parameters
    _paddle_width = paddle_width;
    _paddle_height = paddle_height;
    _ball_size = ball_size; // size of the ball
    _speed = speed; // speed of the ball
    _number_left = 18; // number of bricks that remain on screen
    _index = 0;  // index of which laser to move on screen at any time, starts at 0
    _cool_time = 0.0f; // time remaining before laser can be fired again, starts at 0
    _score = 0; // initialised as 0
    _prev_score = 0;  // initialised as 0 for the first round
    _multiplier = 0;  // initialised as 1 for the first round

    // y position of the paddle on screen - HEIGHT is defined in N5110.h
    _paddley = HEIGHT - GAP - 1;

    // initialises paddles and ball in middle
    _paddle.init(_paddley,_paddle_height,_paddle_width);
    _ball.init(_ball_size,_speed,_paddle.get_pos().x + (PADDLE_WIDTH/2));

    // initialises all the bricks in their set x/y positions
    _brick11.init(3,GAP_TOP+1,3);
    _brick12.init(16,GAP_TOP+1,3);
    _brick13.init(29,GAP_TOP+1,3);
    _brick14.init(42,GAP_TOP+1,3);
    _brick15.init(55,GAP_TOP+1,3);
    _brick16.init(68,GAP_TOP+1,3);

    _brick21.init(3,GAP_TOP+BRICK_HEIGHT+2,2);
    _brick22.init(16,GAP_TOP+BRICK_HEIGHT+2,2);
    _brick23.init(29,GAP_TOP+BRICK_HEIGHT+2,2);
    _brick24.init(42,GAP_TOP+BRICK_HEIGHT+2,2);
    _brick25.init(55,GAP_TOP+BRICK_HEIGHT+2,2);
    _brick26.init(68,GAP_TOP+BRICK_HEIGHT+2,2);

    _brick31.init(3,GAP_TOP+1+((BRICK_HEIGHT+1)*2),1);
    _brick32.init(16,GAP_TOP+1+((BRICK_HEIGHT+1)*2),1);
    _brick33.init(29,GAP_TOP+1+((BRICK_HEIGHT+1)*2),1);
    _brick34.init(42,GAP_TOP+1+((BRICK_HEIGHT+1)*2),1);
    _brick35.init(55,GAP_TOP+1+((BRICK_HEIGHT+1)*2),1);
    _brick36.init(68,GAP_TOP+1+((BRICK_HEIGHT+1)*2),1);

    // appends all bricks to a list so that they are easier to check for collisions
    listofBricks.push_back(_brick11);
    listofBricks.push_back(_brick12);
    listofBricks.push_back(_brick13);
    listofBricks.push_back(_brick14);
    listofBricks.push_back(_brick15);
    listofBricks.push_back(_brick16);
    listofBricks.push_back(_brick21);
    listofBricks.push_back(_brick22);
    listofBricks.push_back(_brick23);
    listofBricks.push_back(_brick24);
    listofBricks.push_back(_brick25);
    listofBricks.push_back(_brick26);
    listofBricks.push_back(_brick31);
    listofBricks.push_back(_brick32);
    listofBricks.push_back(_brick33);
    listofBricks.push_back(_brick34);
    listofBricks.push_back(_brick35);
    listofBricks.push_back(_brick36);

    // initialises lasers off screen
    _laser1.init();
    _laser2.init();
    _laser3.init();

    // appends lasers to a list so that they are easier to check for collisions
    listofLasers.push_back(_laser1);
    listofLasers.push_back(_laser2);
    listofLasers.push_back(_laser3);

    _powerup.init();
}


void BreakoutEngine::reset_game()  // resets the game after victory screen
{
    reset_num_left();   // resets _number_left to 18
    _paddle.recentre();  // recentres the paddle on screen
    //printf("Paddle lives = %d", _paddle.get_lives());
    _ball.init(_ball_size,_speed + _multiplier/2, _paddle.get_pos().x + (PADDLE_WIDTH/2)); // resets the ball position to above the paddle, and increases its speed depending on the _multiplier
    _powerup.set_posx(-50);

    int pointer = 0;  // tracks the rows, if 6 bricks are placed, it moves to the next row
    for (it_R = listofBricks.begin(); it_R != listofBricks.end(); ++it_R) {
        if (pointer <= 5) {  // 6 bricks in the first row
            it_R -> set_posx((pointer * 13) + 3);
        } else if (pointer <= 11) {
            it_R -> set_posx(((pointer-6) * 13) + 3);  // pointer - 6 to offset it back to the first column of bricks
        } else if (pointer <= 17) {
            it_R -> set_posx(((pointer-12) * 13) + 3);  // pointer - 12 to offset it back to the first column of bricks
        }

        it_R -> reset_lives(_multiplier);  // increases the bricks number of lives by the multiplier + their _base_lives
        pointer ++;  // increment the pointer
        //printf("pointer = %d",pointer);
    }
    for (it_L = listofLasers.begin(); it_L != listofLasers.end(); ++it_L) {
        it_L -> set_posx(-10);  // moves all the lasers off screen
    }
}


void BreakoutEngine::read_input(Gamepad &pad)
{
    _d = pad.get_direction();
    _mag = pad.get_mag();

    if (pad.check_event(Gamepad::B_PRESSED) && _cool_time <= 0) {  // if B is pressed and it is not cooling down then fire a laser

        Vector2D p_pos = _paddle.get_pos();
        it_L = listofLasers.begin();
        switch(_index) {     // switch between the lasers each time the button is pressed so that it doesnt just re position the same laser each time
            case 0:
                advance(it_L, 0);
                it_L -> set_posx(p_pos.x+7);  // move laser 1 to middle of paddle
                it_L -> set_posy(p_pos.y);
                inc_index();
                //printf("index = %d",_index);
                break;
            case 1:
                advance(it_L, 1);
                it_L -> set_posx(p_pos.x+7);  // move laser 2 to middle of paddle
                it_L -> set_posy(p_pos.y);
                inc_index();
                //printf("index = %d",_index);
                break;
            case 2:
                advance(it_L, 2);
                it_L -> set_posx(p_pos.x+7);  // move laser 3 to middle of paddle
                it_L -> set_posy(p_pos.y);
                //printf("index = %d",_index);
                reset_index();
                //printf("index = %d",_index);
                break;
        }

        it_L = listofLasers.end();

        _cool_time = 0.75f;  // reset cool time to 0.75 seconds
    } else {
        _cool_time -= 0.125; // reduce cool down time by 1/8 of a second as fps is 8  
    }

}


void BreakoutEngine::draw(N5110 &lcd)
{
    lcd.drawRect(0,GAP_TOP - 2,WIDTH,HEIGHT - GAP_TOP + 2,FILL_TRANSPARENT); // draw the game screen dimensions
    print_scores(lcd);  // print the score above the screen
    _paddle.draw(lcd);  // draw the paddle
    _ball.draw(lcd);  // draw the ball

    for (it_R = listofBricks.begin(); it_R != listofBricks.end(); ++it_R) {  // draw all teh bricks
        it_R->draw(lcd);
    }
    for (it_L = listofLasers.begin(); it_L != listofLasers.end(); ++it_L) {  // draw all the lasers
        it_L->draw(lcd);
    }

    check_life_powerup();  // check if power up can be drawn, if so move on screen when random condition is met

    _powerup.draw(lcd);   // draw the powerup
}


void BreakoutEngine::update(Gamepad &pad)
{
    check_loss(pad);  // check if the ball has gone above max y
    
    _paddle.update(_d,_mag);  // update paddle position
    _ball.update();   // update ball position
    _powerup.update();  // update powerup position

    for (it_L = listofLasers.begin(); it_L != listofLasers.end(); ++it_L) { // update all the lasers' positions
        it_L->update();
    }

    lives_leds(pad);  // update the LEDs with the remaining lives

    check_wall_collisions(pad);  // check ball and laser collisions with walls and ceiling
    check_paddle_collisions(pad);  // check ball collision with paddle
    check_brick_collisions(pad);  // check ball collision with bricks
    check_laser_collisions(pad);  // check all laser collisions with all bricks
    check_powerup_collisions(pad);  // check powerup collision with paddle
}


void BreakoutEngine::lives_leds(Gamepad &pad)  // converst the number of lives left into LEDs
{
    if (_paddle.get_lives() == 0) {  
        pad.leds_off();  // all LEDs off
    }

    else if (_paddle.get_lives() == 1) {
        pad.leds_off();
        pad.led(1,1);   // 1 LED on
    }

    else if (_paddle.get_lives() == 2) {
        pad.leds_off();
        pad.led(1,1);
        pad.led(2,1); // 2 LEDs on
    }

    else if (_paddle.get_lives() == 3) {
        pad.leds_off();
        pad.led(1,1);
        pad.led(2,1);
        pad.led(3,1);  // 3 LEDs on
    }

    else if (_paddle.get_lives() == 4) {
        pad.leds_on();
        pad.led(5,0);
        pad.led(6,0); // 4 LEDs on
    }

    else if (_paddle.get_lives() == 5) {
        pad.leds_on();
        pad.led(6,0);  // 5 LEDs on
    }

    else if (_paddle.get_lives() == 6) {
        pad.leds_on();  // all LEDs on
        //printf("all LEDs on \n");
    }
}


void BreakoutEngine::check_wall_collisions(Gamepad &pad)  // checks ball and laser collisions with the walls and ceiling
{
    // read current ball attributes
    Vector2D ball_pos = _ball.get_pos();
    Vector2D ball_velocity = _ball.get_velocity();
    
    // bounce off the ceiling
    if (ball_pos.y <= GAP_TOP - 1) {  //  1 due to 1 pixel boundary
        ball_pos.y = GAP_TOP - 1;  
        ball_velocity.y = -ball_velocity.y;
        pad.tone(750.0,0.1);
    }
    
    // bounce off the left
    else if (ball_pos.x <= 1) {  //  1 due to 1 pixel boundary
        ball_pos.x = 1;  // bounce off walls 
        ball_velocity.x = -ball_velocity.x;
        pad.tone(750.0,0.1);
    }

    // bounce off the right
    else if (ball_pos.x + _ball_size >= (WIDTH-1) ) { // bottom pixel is 47
        // hit bottom
        ball_pos.x = (WIDTH-1) - _ball_size;  // stops ball going off screen
        ball_velocity.x = -ball_velocity.x;
        // audio feedback
        pad.tone(750.0,0.1);
    }

    // update ball parameters
    _ball.set_velocity(ball_velocity);
    _ball.set_pos(ball_pos);

    for (it_L = listofLasers.begin(); it_L != listofLasers.end(); ++it_L) { // checks every laser
        if (
            (it_L -> get_y() <= GAP_TOP - 2) // if it hits the ceiling 
        ) {    
            it_L -> set_posx(-10);  // set its x coordinate off screen
        }
    }

}


void BreakoutEngine::check_paddle_collisions(Gamepad &pad)
{
    // read current ball attributes
    Vector2D ball_pos = _ball.get_pos();
    Vector2D ball_velocity = _ball.get_velocity();

    // read the paddle position
    Vector2D paddle_pos = _paddle.get_pos();

    // check if ball overlaps with the paddle
    if (
        (ball_pos.x >= paddle_pos.x) && //left
        (ball_pos.x <= paddle_pos.x + _paddle_width) && //right
        (ball_pos.y >= _paddley) && //bottom
        (ball_pos.y <= _paddley + _paddle_height)  //top
    ) {    
        pad.tone(1000.0,0.1);
        ball_pos.y = _paddley + _paddle_height - 1;
        ball_velocity.y = -ball_velocity.y;
        
        // below is a method to control the ball reflect angle based on where it hits the paddle
        // however cannot be used as it does not work with collisions due to varying angles and speeds
        // would work with greater pixel density in the screen

//        if (ball_pos.x == paddle_pos.x + PADDLE_WIDTH/2) {     // check ballxpos in relation to paddle xpos. translate the distance from the centre to an angle between 30 and 60 degrees in that direction
//            ball_pos.y = _paddley + _paddle_height - 1;
//            ball_velocity.y = -ball_velocity.y;
//        }
//        else if (ball_pos.x <= paddle_pos.x + PADDLE_WIDTH/2) {
//            float ang = 40*(((paddle_pos.x + PADDLE_WIDTH/2)-ball_pos.x)/(PADDLE_WIDTH/2 - paddle_pos.x)) + 30;  //converts the distance from the centre to an angle between 30 and 60
//            if (ball_velocity.x > 0) {
//                ball_velocity.x = -ball_velocity.x;
//            }
//           ball_pos.y = _paddley + _paddle_heigh - 1;
//           ball_velocity.y = -tan(ang);
//       }
//       else if (ball_pos.x >= paddle_pos.x + PADDLE_WIDTH/2) {
//           float ang = 40*(((paddle_pos.x + PADDLE_WIDTH/2)-ball_pos.x)/(PADDLE_WIDTH/2 - paddle_pos.x)) + 30;  //converts the distance from the centre to an angle between 30 and 60
//           if (ball_velocity.x < 0) {
//               ball_velocity.x = -ball_velocity.x;
//           }
//           ball_pos.y = _paddley + _paddle_height - 1;
//           ball_velocity.y = -tan(ang);
//       }
    }

    // write new attributes
    _ball.set_velocity(ball_velocity);
    _ball.set_pos(ball_pos);
}

void BreakoutEngine::check_brick_collisions(Gamepad &pad)
{
    // read current ball attributes
    Vector2D ball_pos = _ball.get_pos();
    Vector2D ball_velocity = _ball.get_velocity();

    for (it_R = listofBricks.begin(); it_R != listofBricks.end(); ++it_R) {
        if (
            (ball_pos.x >= it_R -> get_x()) && //left
            (ball_pos.x <= it_R -> get_x() + BRICK_WIDTH) && //right
            (ball_pos.y >= it_R -> get_y()) && //bottom
            (ball_pos.y <= it_R -> get_y() + BRICK_HEIGHT)  //top
        ) {    
            if (ball_velocity.y < 0) {  // if velocity is negative, it has hit the bottom, therefore correct its pos to the bottom face
                ball_pos.y = it_R -> get_y() + BRICK_HEIGHT;
            } else {  // else it has hit the top, correct its position to the top face
                ball_pos.y = it_R -> get_y();   
            }
            ball_velocity.y = -ball_velocity.y;  // invert the balls y velocity
            // audio feedback
            pad.tone(1000.0,0.1);  
            it_R -> hit();  
            if(it_R-> hit() == true) {  // hit the brick, remove 2 lives from it and check if destroyed
                it_R -> set_posx(-100);  // if destroyed, move off screen
                dec_num_left();  // decrement the number of bricks left on screen
            }
        }
    }
    // write new attributes
    _ball.set_velocity(ball_velocity);
    _ball.set_pos(ball_pos);
}


void BreakoutEngine::check_laser_collisions(Gamepad &pad)
{
    for (it_L = listofLasers.begin(); it_L != listofLasers.end(); ++it_L) {   
        for (it_R = listofBricks.begin(); it_R != listofBricks.end(); ++it_R) {  // check every laser against every brick
            if (
                (it_L -> get_x() >= it_R -> get_x()) && //left
                (it_L -> get_x() <= it_R -> get_x() + BRICK_WIDTH) && //right
                (it_L -> get_y() >= it_R -> get_y()) && //bottom
                (it_L -> get_y() <= it_R -> get_y() + BRICK_HEIGHT)  //top
            ) {  
                it_L -> set_posx(-10);    // if they overlap/collide, move the laser off screen
                // audio feedback
                pad.tone(1000.0,0.1);  
                if(it_R-> hit() == true) {   // remove a life from the brick, if its destroyed, move it off screen
                    it_R -> set_posx(-100);
                    dec_num_left();  // decrement the number of bricks remaining on screen
                }
            }
        }
    }
}


void BreakoutEngine::check_powerup_collisions(Gamepad &pad)  // checks powerup collisions with the paddle and the bottom of the screen
{
    // read the current position of the paddle
    Vector2D paddle_pos = _paddle.get_pos();  
    
    if (
        (_powerup.get_x() >= paddle_pos.x) && //left
        (_powerup.get_x() + 8 <= paddle_pos.x + _paddle_width) && //right
        (_powerup.get_y() + 8 >= paddle_pos.y) // top
    ) {   
        pad.tone(2500.0,0.1);
        _powerup.set_posx(-50);   // if the paddle and power up overlap, move the powerup off screen
        _paddle.inc_life();   // increment the numebr of lives left on the paddle
    }

    else if (
        (_powerup.get_y() +9 >= HEIGHT)  // bottom of screen
    ) {    
        _powerup.set_posx(-50);   // if the powerup reaches the bottom of the screen, move it off screen
    }
}


bool BreakoutEngine::check_loss(Gamepad &pad)  // check if the ball has hit the bottom of the screen
{
    // read the current ball position
    Vector2D ball_pos = _ball.get_pos();
    
    if (ball_pos.y > HEIGHT) {  // if it has gone off screen
        _paddle.lose_life();   // decrement the number of lives left

        _ball.init(_ball_size,_speed+_multiplier/2,_paddle.get_pos().x + (PADDLE_WIDTH/2));  // re initialise the ball above the paddle
        pad.tone(1500.0,0.5);
        return true;     // return true, to be used to flash the screen in the main
    } else {
        return false;    
    }
}


void BreakoutEngine::set_paddle_motion(bool tilt, float sens)  // sets the motion options for the paddle
{
    if (tilt == true) {
        _paddle.set_tilt();  // sets tilt
    } else {
        _paddle.set_joy();  // sets joystick
    }
    _paddle.set_sens(sens);  // sets sensitivity
}


void BreakoutEngine::print_scores(N5110 &lcd)
{
    int score = _prev_score + (18 - get_num_left())*100 * (_multiplier + 1);  // current score depends on number left, the multiplier and the previous score
    _score = score; // set the member variable _score
    
    char buffer[14];
    sprintf(buffer,"%2d",score); // put score in buffer
    lcd.printString("SCORE: ",2,0);
    lcd.printString(buffer,WIDTH/2 -2,0);  // print buffer on LCD
}


void BreakoutEngine::check_life_powerup()
{
    srand(time(0));  // Initialize random number generator.
    int r1 = (rand() % 25) + 1; 
    int r2 = (rand() % 25) + 1;   // 2 random number between 1 and 26
    int rx = (rand() % 60) + 11;   // generate a random x coordinate for the powerup to be place
    //printf("r1 = %d",r1);
    //printf("r2 = %d",r2);
    //printf("rx = %d",rx);
    if ((get_paddle_lives() < 6) & (r1 == r2)) {   // if lives are not max, and the random condition is met
        _powerup.set_posx(rx);   // set the powerup x
        _powerup.set_posy(HEIGHT/2);  // set the y at the middle of the screen
    }
}


int BreakoutEngine::get_prev_score()
{
    return _prev_score;  // return the previous score
}


void BreakoutEngine::set_prev_score(int prev_score)
{
    _prev_score = prev_score;  // set the member variable _prev_score
}


int BreakoutEngine::get_num_left()
{
    return _number_left;  // return the number of bricks left on screen
}


void BreakoutEngine::dec_num_left()
{
    _number_left -= 1;  // decrement the member variable _number_left when a brick is destroyed
}


void BreakoutEngine::reset_num_left()
{
    _number_left = 18;  // reset the number left to 18 when the game is reset
}


int BreakoutEngine::get_score()
{
    return _score;  // return the member variable _score
}


void BreakoutEngine::inc_mult()
{
    _multiplier ++;   // increment the _multiplier when continue is selected upon victory
}


int BreakoutEngine::get_mult()
{
    return _multiplier;   // retrieve the multiplier value
}


void BreakoutEngine::reset_mult()
{
    _multiplier = 0;    // reset the multiplier to 0
}


void BreakoutEngine::inc_index()
{
    _index ++;    // increment the laser index
}


void BreakoutEngine::reset_index()
{
    _index = 0;  // reset the laser index
}


void BreakoutEngine::reset_paddle_lives()
{
    _paddle.reset_lives();  // resets paddle lives
}


int BreakoutEngine::get_paddle_lives()
{
    return _paddle.get_lives();    // return the number of lives left
}
