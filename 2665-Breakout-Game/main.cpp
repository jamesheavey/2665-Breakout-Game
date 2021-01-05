///////// pre-processor directives ////////

#include "mbed.h"
#include "Gamepad.h"
#include "N5110.h"
#include "BreakoutEngine.h"
#include "Bitmap.h"
#include "Sprites.h"
#include "SDFileSystem.h"

#ifdef WITH_TESTING
# include "tests.h"
#endif

/////////////// structs /////////////////

struct UserInput {
    Direction d;
    float mag;
};

/////////////// objects ///////////////

N5110 lcd(PTC9,PTC0,PTC7,PTD2,PTD1,PTC11);
Gamepad pad;
BreakoutEngine breakout;
SDFileSystem sd(PTE3, PTE1, PTE2, PTE4, "sd"); // MOSI, MISO, SCK, CS
Serial serial(USBTX, USBRX);  // for PC debug

///////////// prototypes ///////////////

void init();
void update_game(UserInput input);
void render();
void main_menu();
void settings();
void how_to_play();
void loss_screen();
void victory_screen();
void title_screen();
void main_game();
void flash_screen(N5110 &lcd);
void countdown();
void save_hi_score(int hi_score);
int get_hi_score();
bool compare_to_hi_score(int score);
void print_hi_score(int col,int row);
void flash_hi_score_screen();
void reset_victory(int score, int bonus);
void reset_loss();

Bitmap breakwhite(breakwhite_data, 48, 84);  // assign the title screen sprites
Bitmap breakblack(breakblack_data, 48, 84);
Bitmap arrowup(arrowup_data, 5, 7);  // assign the arrow up sprite data
Bitmap arrowdown(arrowdown_data, 5, 7);  // assign the arrow down sprite data
Bitmap three(three_data, 48, 84);  // assign the 3 sprite data
Bitmap two(two_data, 48, 84);  // assign the 2 sprite data
Bitmap one(one_data, 48, 84);  // assign the 1 sprite data

bool tilt = false;
float sens = 0.5; // default sens
int number_of_frames = 0; // tracks the number of frames passed to use for bonus score

///////////// functions ////////////////

int main()
{
    
#ifdef WITH_TESTING
    int number_of_failures = run_all_tests();

    if(number_of_failures > 0) return number_of_failures;
#endif

    init();     // initialise the gamepad
    title_screen();  // run the title screen
}




void init()  // initialies all classes and libraries
{
    // need to initialise LCD and Gamepad
    lcd.init();
    pad.init();
    breakout.init(PADDLE_WIDTH,PADDLE_HEIGHT,BALL_SIZE,BALL_SPEED); // init the objects
}

void render()  // draws each frame on the LCD
{
    // clear screen, re-draw and refresh
    lcd.clear();
    breakout.draw(lcd);
    lcd.refresh();
}


//////////// MENUING & GAME LOOP FUNCTIONS //////////////////


void title_screen()
{
    tilt = false;   // reset the tilt variable so that on start, joystick is default
    breakout.reset_mult();
    lcd.clear();

    lcd.setBrightness(1); // stops the game from dimming (hardware bug?)

    breakblack.render(lcd, 0, 0);  // render the first frame
    lcd.refresh();
    pad.leds_off();
    wait(0.5);
    
    pad.reset_flags();

    while (pad.check_event(Gamepad::START_PRESSED) == false) {  // while start is not pressed alternate sprites

        lcd.clear();

        breakwhite.render(lcd, 0, 0);
        lcd.refresh();
        pad.leds_on();
        wait(0.5);

        lcd.clear();

        breakblack.render(lcd, 0, 0);
        lcd.refresh();
        pad.leds_off();
        wait(0.5);
    }

    pad.tone(750.0,0.3);
    wait(0.2);
    main_menu();  // load main menu
}


void main_menu()
{
    lcd.setBrightness(1);  // fixes random screen dimming (hardware bug?)
    
    lcd.clear();
    lcd.printString("   START ",0,1);    // start game with default as joystick
    lcd.printString("   SETTINGS ",0,2);   // choose between joystick and tilt
    lcd.printString("   HOW TO PLAY ",0,3);  // brief text on how to interact with the game
    lcd.printString("HI-SCORE: ",0,5);
    print_hi_score(55,5);
    lcd.refresh();
    wait(0.3);  // load initial frame 
    
    pad.reset_flags();  // manually resets all button flags which fixes button bounce and prevents inputs carrying through to different menus
    pad.leds_off(); // prevents hardware bug where right most led flickers
    
    int pointer = 1;

    while (pad.check_event(Gamepad::A_PRESSED) == false) {
        lcd.clear();
        lcd.printString("   START ",0,1);    // start game with default as joystick
        lcd.printString("   SETTINGS ",0,2);   // choose between joystick and tilt
        lcd.printString("   HOW TO PLAY ",0,3);  // brief text on how to interact with the game
        lcd.printString("HI-SCORE: ",0,5);
        print_hi_score(55,5);
        lcd.printString(" >",0,pointer);
        lcd.refresh();
        wait(0.1);
        if (pad.get_direction() == N && pointer > 1) {  // if L is pressed and pointer isnt already on START, move it up one line
            pointer -= 1;
            pad.tone(750.0,0.3);
            wait(0.1);   
        } else if (pad.get_direction() == S && pointer < 3) {  // if R is pressed and pointer isnt already on HOW TO PLAY, move it down one line
            pointer += 1;
            pad.tone(750.0,0.3);
            wait(0.1);  
        }
        if (pad.check_event(Gamepad::X_PRESSED) & pad.check_event(Gamepad::Y_PRESSED)) {
            save_hi_score(0);  // resets hi score
        }
        //printf("Pointer 1 = %d",pointer);

    }
    if (pointer == 1) {    // if START was selected on exit of the while loop, run main game with the appropriate tilt/joystick setting
        pad.tone(750.0,0.3);
        number_of_frames = 0;
        main_game();
    } else if (pointer == 2) { // if SETTINGS was selected, enter the settings menu
        pad.tone(750.0,0.3);
        settings();
    } else if (pointer == 3) { // if HOW TO PLAY was selected, display instructions on how to play
        pad.tone(750.0,0.3);
        how_to_play();
    }
}

void settings()
{
    lcd.clear();    // load initial frame
    lcd.printString("   JOYSTICK ",0,1);    // choose joystick
    lcd.printString("   TILT ",0,2);   // choose tilt
    lcd.printString("SENS :",0,4);
    lcd.drawRect(42,30, 40,10,FILL_TRANSPARENT);
    lcd.drawRect(42,30,40 * pad.read_pot() + 1,10,FILL_BLACK); // represents the sensitivity which is changed by turning the pot
    lcd.refresh();
    wait(0.1);   
    
    pad.reset_flags();  // fixes button bounce

    int pointer = 1;  // init the pointer

    while (pad.check_event(Gamepad::B_PRESSED) == false) { // while B is not pressed to return to main menu
        lcd.clear();
        lcd.printString("   JOYSTICK ",0,1);    // choose joystick
        lcd.printString("   TILT ",0,2);   // choose tilt
        lcd.printString("SENS :",0,4);
        lcd.printString(" >",0,pointer);
        lcd.drawRect(42,30, 40,10,FILL_TRANSPARENT);
        lcd.drawRect(42,30,40 * pad.read_pot() + 1,10,FILL_BLACK); // have it so it fills half the transparent one (default position)
        lcd.refresh();
        wait(0.1);
        if (pad.get_direction() == N && pointer > 1) {  // if L is pressed and pointer isnt already on JOYSTICK, move it up one line
            pointer -= 1;
            pad.tone(750.0,0.3);
            wait(0.1);  
        } else if (pad.get_direction() == S && pointer < 2) {  // if R is pressed and pointer isnt already on TILT, move it down one line
            pointer += 1;
            pad.tone(750.0,0.3);
            wait(0.1);  
        }

        if (pad.check_event(Gamepad::A_PRESSED)) { // if A is pressed, switch the tilt option accordingly
            pad.tone(750.0,0.3);
            wait(0.1);
            if (pointer == 1) {  // if A is pressed on JOYSTICK, tilt = false
                tilt = false;
            } else if (pointer == 2) { // if A is pressed on TILT, tilt == true
                tilt = true;
            }
        }

        if (pad.check_event(Gamepad::B_PRESSED)) {  // when B is pressed return to main menu
            pad.tone(750.0,0.3);
            sens = pad.read_pot() + 0.5f; // sens is set as the pot value at the instant the settings menu is exited +the minimum value of 0.5
            breakout.set_paddle_motion(tilt,sens); // sets the paddles motion options
            //printf("Sensitivity = %d",sens);
            wait(0.3);
            main_menu();
        }
        //printf("Pointer 2 = %d",pointer);
    }
}

void how_to_play()  // explains how to interact with the game
{
    lcd.clear();
    lcd.printString("   B = LASER  ",0,0);
    lcd.printString(" START = PAUSE  ",0,1);
    lcd.printString("  DESTROY ALL   ",0,2);
    lcd.printString("    BRICKS. ",0,3);
    arrowdown.render(lcd, 38, 43);
    lcd.refresh();
    wait(0.1);           // load initial frame 
    
    pad.reset_flags();  // fixes button bounce

    while (pad.check_event(Gamepad::B_PRESSED) == false) { // while B is not pressed to return to main menu, display instruction on how to interact with the game

        if (pad.get_direction() == S) {
            lcd.clear();
            lcd.printString(" CONTINUE TO  ",0,2);
            lcd.printString("INCREASE SCORE.  ",0,3);
            lcd.printString(" BONUS SCORE  ",0,4);
            lcd.printString("BASED ON TIME.  ",0,5);
            arrowup.render(lcd, 38, 0);
            lcd.refresh();
            wait(0.1);
        }
        if (pad.get_direction() == N) {
            lcd.clear();
            lcd.printString("   B = LASER  ",0,0);
            lcd.printString(" START = PAUSE  ",0,1);
            lcd.printString("  DESTROY ALL   ",0,2);
            lcd.printString("    BRICKS ",0,3);
            arrowdown.render(lcd, 38, 43);
            lcd.refresh();
            wait(0.1);
        }
    }
    wait(0.3);
    main_menu();  // when B is pressed, the loop is exited and main menu is entered once again
}


void main_game()    // the Game loop
{   
    pad.reset_flags();  // fixes button bounce
    
    int fps = 8;  // frames per second
    bool pause = false; // set pause screen to false
    //printf("Pause = %d",pointer);

    countdown(); // run the countdown

    render();  // first draw the initial frame
    wait(1.0f/fps);  // and wait for one frame period


    // game loop - read input, update the game state and render the display
    while (1) {
        breakout.read_input(pad); // read input from pad
        breakout.update(pad);  // update game
        lcd.setBrightness(1); // stops the game from dimming (bug)
        render();  // draw new frame

        if (breakout.check_loss(pad) == true) { // if life lost flash screen
            flash_screen(lcd);
        }
        if (pad.check_event(Gamepad::START_PRESSED)) { // if BACK pressed, toggle pause
            pause = !pause;
            pad.reset_flags();  // fixes button bounce?
        }

        while (pause == true) { // if pause is true, display pause screen
            lcd.clear();
            lcd.printString("    PAUSED ",0,1);
            lcd.printString(" START = PLAY",0,3);
            lcd.printString("  BACK = MENU",0,4);
            lcd.refresh();
            wait(0.1);
            if (pad.check_event(Gamepad::START_PRESSED)) { // if START pressed, toggle pause, leaving pause screen
                pause = !pause;
            }
            if (pad.check_event(Gamepad::BACK_PRESSED)) { // if BACK pressed, return to the title screen
                reset_loss();
                title_screen();
            }
        }

        if (breakout.get_paddle_lives() == 0) { // when all lives lost, enter loss screen
            pad.leds_off(); //turns last led off (doesnt run through and update board so last led doent turn off via lives leds
            loss_screen();
        }

        if (breakout.get_num_left() == 0) { // when all bricks destroyed, display victory screen
            victory_screen();
        }
        number_of_frames ++; // track the number of frames passed to add to the score (inversely proportional)
        wait(1.0f/fps); // wait for 1 frame
    }
}


void loss_screen()   // loss screen when lives of paddle == 0
{
    if (compare_to_hi_score(breakout.get_score()) == true) {  // little hi-score sequence if current score is higher than prev
        flash_hi_score_screen();
    }

    lcd.clear();
    char buffer[14]; // init the buffer
    sprintf(buffer,"%2d",breakout.get_score());  // print the score to the buffer
    lcd.printString(buffer,WIDTH/2 - 12,2);  // print buffer on lcd
    lcd.printString("   RESTART? ",2,1);
    lcd.printString("  PRESS START ",1,4);
    lcd.refresh();
    pad.tone(750.0,0.3);
    wait(0.4);

    pad.tone(300.0,0.3);
    wait(0.4);
    
    pad.reset_flags();

    while (pad.check_event(Gamepad::START_PRESSED) == false) {  // flashes the score, waits for START to return to title screen
        lcd.clear();

        lcd.printString("   RESTART? ",2,1);
        lcd.printString("  PRESS START ",1,4);
        lcd.refresh();

        wait(0.4);

        lcd.clear();

        lcd.printString(buffer,WIDTH/2 - 12,2);
        lcd.printString("   RESTART? ",2,1);
        lcd.printString("  PRESS START ",1,4);
        lcd.refresh();

        wait(0.4);
    }
    reset_loss();
    title_screen();
}


void victory_screen() // victory screen when all bricks are destroyed
{
    int bonus = NULL;

    if (breakout.get_score()/2 - ((breakout.get_score()/2) * number_of_frames)/720 > 0) {   // beat within 90 seconds (8 fps) and you get a bonus
        bonus = breakout.get_score()/2 - ((breakout.get_score()/2) * number_of_frames)/720;
    } else {
        bonus = 0; // else you get no bonus
    }

    if (compare_to_hi_score(bonus+breakout.get_score()) == true) {   // little hi-score sequence if current score is higher than prev
        flash_hi_score_screen();
    }

    lcd.clear();

    char buffer1[14];
    sprintf(buffer1,"%2d",breakout.get_score());
    lcd.printString(buffer1,WIDTH/2 - 12,2);

    char buffer2[14];
    sprintf(buffer2,"%2d",bonus);
    lcd.printString(buffer2,WIDTH/2 + 12,3);
    lcd.printString(" BONUS: ",2,3);

    lcd.printString("   VICTORY! ",2,0);
    lcd.printString(" CONT = START ",0,4);
    lcd.printString("  MENU = BACK ",0,5);
    lcd.refresh();
    pad.tone(2500.0,0.1);
    wait(0.4);

    pad.tone(2500.0,0.1);
    wait(0.2);

    pad.tone(4000.0,0.6);
    wait(0.6);
    
    pad.reset_flags();

    while (pad.check_event(Gamepad::START_PRESSED) || pad.check_event(Gamepad::BACK_PRESSED) == false) {  // while neither START or BACK is pressed, flash the score and display options
        lcd.clear();

        lcd.printString("   VICTORY! ",2,0);     
        lcd.printString(" CONT = START ",0,4);
        lcd.printString("  MENU = BACK ",0,5);

        lcd.refresh();

        wait(0.4);

        lcd.clear();

        lcd.printString(buffer1,WIDTH/2 - 12,2);
        lcd.printString(buffer2,WIDTH/2 + 12,3);
        lcd.printString(" BONUS:",2,3);

        lcd.printString("   VICTORY! ",2,0);
        lcd.printString(" CONT = START ",0,4);
        lcd.printString("  MENU = BACK ",0,5);
        lcd.refresh();

        pad.tone(2500.0,0.1);

        lcd.refresh();

        wait(0.4);

        if (pad.check_event(Gamepad::START_PRESSED)) {
            reset_victory(breakout.get_score(),bonus);  // reset game
            main_game();  // reload game
        } else if (pad.check_event(Gamepad::BACK_PRESSED)) {
            reset_loss();    // reset game
            title_screen();  // load title screen
        } 
    }
}


void flash_screen(N5110 &lcd)   // flash the screen when a life is lost
{
    lcd.setBrightness(0);
    wait(0.1);
    lcd.setBrightness(1);
    wait(0.1);
    lcd.setBrightness(0);
    wait(0.1);
    lcd.setBrightness(1);
    wait(0.1);
    lcd.setBrightness(0);
    wait(0.1);
    lcd.setBrightness(1);
    wait(0.1);
    lcd.setBrightness(0);
    wait(0.1);
    lcd.setBrightness(1);
}

void countdown()       // draw the countdown
{
    lcd.setBrightness(1); // stops the game from dimming (bug)

    lcd.clear();
    three.render(lcd, 0, 0);   // render the 3
    lcd.refresh();
    pad.tone(500.0,0.5);
    wait(1);    // wait 1 second

    lcd.setBrightness(1);

    lcd.clear();
    two.render(lcd, 0, 0);   // render 2
    lcd.refresh();
    pad.tone(500.0,0.5);
    wait(1);    // wait 1 second

    lcd.setBrightness(1);

    lcd.clear();
    one.render(lcd, 0, 0);  // render 1
    lcd.refresh();
    pad.tone(1000.0,1);
    wait(1);     // wait 1 second
}


//////////////// SD CARD FUNCTIONS ////////////////////////


void save_hi_score(int hi_score)  // save score to SD card
{
    serial.baud(115200);  // max speed
    FILE *fp; // file pointer
    fp = fopen("/sd/hi_score.txt", "w");
    if (fp == NULL) {  // if it can't open the file then print error message
        serial.printf("Error\n");
    } else {  // opened file so can write
        fprintf(fp, "%d",hi_score);
        serial.printf("Written to file.\n");
        fclose(fp);  // close the file after writing
    }
}


int get_hi_score()  // retrieve score from SD card
{
    serial.baud(115200); // max speed
    FILE *fp;
    fp = fopen("/sd/hi_score.txt", "r");
    int stored_hi_score = NULL;
    if (fp == NULL) {  // if it can't open the file then print error message
        serial.printf("Error\n");
    } else {  // opened file so can write
        fscanf(fp, "%d",&stored_hi_score);
        serial.printf("Read %d from file.\n",stored_hi_score);
        fclose(fp);  // close the file after reading
    }
    return stored_hi_score;  // returns the stored hi score
}


bool compare_to_hi_score(int score)  // returns true if new score higher than current stored hi score
{
    if (score >= get_hi_score()) {
        //printf("hi score! \n");
        save_hi_score(score);  // writes the new score to the SD card
        return true;
    } else {
        return false;
    }
}


void print_hi_score(int col,int row)  // print hi score to lcd at specified location
{
    char buffer[14];  // creates buffer
    sprintf(buffer,"%2d",get_hi_score()); // puts hi-score in buffer
    lcd.printString(buffer,col,row);   // prints buffer to the screen

}


void reset_loss()  // reset the game when returning to title screen from any point
{
    breakout.reset_paddle_lives(); // resets lives back to 6
    breakout.set_prev_score(0);   // resets prev score to 0
    number_of_frames = 0;  // reset the number of frames
    breakout.reset_mult(); // reset multiplier
    breakout.reset_game(); // return game to initial positions
}


void reset_victory(int score, int bonus)  // reset the game after a victory
{
    breakout.set_prev_score(score + bonus);  // saves score
    number_of_frames = 0;  // reset the number of frames
    breakout.inc_mult();  // increment multiplier
    breakout.reset_game(); // return game to initial positions
}


void flash_hi_score_screen()  // flash the hi score, called when hi score acheieved
{
    lcd.clear();
    lcd.printString("      NEW ",0,2);
    lcd.printString("   HI-SCORE! ",0,3);
    lcd.refresh();

    pad.tone(2500.0,0.2);
    wait(0.2);

    pad.tone(4000.0,0.4);
    wait(1);

    lcd.clear();
    print_hi_score(30,3);
    lcd.refresh();

    pad.tone(2500.0,0.2);
    wait(0.2);

    pad.tone(4000.0,0.4);
    wait(1);

    lcd.clear();
    lcd.printString("      NEW ",0,2);
    lcd.printString("   HI-SCORE! ",0,3);
    lcd.refresh();

    pad.tone(2500.0,0.2);
    wait(0.2);

    pad.tone(4000.0,0.4);
    wait(1);

    lcd.clear();
    print_hi_score(30,3);
    lcd.refresh();

    pad.tone(2500.0,0.2);
    wait(0.2);

    pad.tone(4000.0,0.4);
    wait(1.3);
}