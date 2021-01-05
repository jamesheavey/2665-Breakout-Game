#ifndef BALL_TEST_H
#define BALL_TEST_H

/**
 * \brief Check that Ball object goes to correct position when moved
 * 
 * \returns true if all the tests passed
 */
bool Ball_test_movement()
{
    // Initialise Ball object with a size of 2, and speed of 2
    Ball _ball;
    _ball.init(2, 2);

    // Set the position to 0, 0
    Vector2D initial_pos = {0, 0};
    _ball.set_pos(initial_pos);

    // Read the position
    Vector2D read_pos_1 = _ball.get_pos();
    printf("%f, %f\n", read_pos_1.x, read_pos_1.y);

    // Set the velocity to -2, 3
    Vector2D velocity = {-2, 3};
    _ball.set_velocity(velocity);

    // Update the position
    _ball.update();

    // Read the position
    Vector2D read_pos_2 = _ball.get_pos();
    printf("%f, %f\n", read_pos_2.x, read_pos_2.y);
    
    // Now check that both the positions are as expected
    bool success_flag = true;
    
    // Fail the test if the initial position is wrong
    if (read_pos_1.x != 0 || read_pos_1.y != 0) {
        success_flag = false;
    }
    
    // Fail the test if the final position is wrong
    if (read_pos_2.x != -2 || read_pos_2.y != 3) {
        success_flag = false;
    }

    return success_flag;
}
#endif