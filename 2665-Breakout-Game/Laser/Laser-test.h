#ifndef LASER_TEST_H
#define LASER_TEST_H

/**
 * \brief Check that Laser object goes to correct position when moved
 * 
 * \returns true if all the tests passed
 */
bool Laser_test_movement()
{
    // Initialise Laser object at -10,0 with velocity of y as -2
    Laser _laser;
    _laser.init();
    
    int read_pos1x = _laser.get_x();
    int read_pos1y = _laser.get_y();
    
     // Read the position
    printf("%f, %f\n", read_pos1x, read_pos1y);

    // Set the x position to 10, 0
    _laser.set_posx(10);

    // Update the position
    _laser.update();

    int read_pos2x = _laser.get_x();
    int read_pos2y = _laser.get_y();
    
     // Read the position
    printf("%f, %f\n", read_pos2x, read_pos2y);
    
    // Now check that both the positions are as expected
    bool success_flag = true;
    
    // Fail the test if the initial position is wrong
    if (read_pos1x != 0 || read_pos1y != 0) {
        success_flag = false;
    }
    if (read_pos2x != 10 || read_pos2y != 0) {
        success_flag = false;
    }
    
    

    return success_flag;
}
#endif