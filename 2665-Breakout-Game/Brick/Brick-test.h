#ifndef BRICK_TEST_H
#define BRICK_TEST_H

/**
 * \brief Check that Brick object goes to correct position when moved
 *
 * \returns true if all the tests passed
 */
bool Brick_test_movement()
{
    // Initialise Brick object at (2,2) with 2 lives
    Brick _brick;
    _brick.init(2, 2, 2);
    int read_pos1x = _brick.get_x();
    int read_pos1y = _brick.get_y();

    // Read the position
    printf("%f, %f\n", read_pos1x, read_pos1y);

    // Set the x position to 10, 0
    _brick.set_posx(10);

    // Update the position
    _brick.update();

    int read_pos2x = _brick.get_x();
    int read_pos2y = _brick.get_y();

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