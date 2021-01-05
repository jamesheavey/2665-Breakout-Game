#ifndef TESTS_H
#define TESTS_H

#include "Ball-test.h"
#include "Brick-test.h"
#include "Laser-test.h"

/**
 * @brief Run unit tests for relevant objects in the Breakout game
 *
 * @returns The number of failed tests
 */
int run_all_tests()
{
    int n_tests_failed = 0; // initialised at 0

    // Run the Ball_test_movement test
    printf("Testing Ball_test_movement...\n");
    bool test_passed1 = Ball_test_movement();

    // Print out the result of this test
    if (test_passed1) {
        printf("...Passed!\n");
    }
    else {
        printf("...FAILED!\n");
        ++n_tests_failed; // Increment number of failures
    }
    
    // Run the Ball_test_movement test
    printf("Testing Brick_test_movement...\n");
    bool test_passed2 = Brick_test_movement();

    // Print out the result of this test
    if (test_passed2) {
        printf("...Passed!\n");
    }
    else {
        printf("...FAILED!\n");
        ++n_tests_failed; // Increment number of failures
    }
    
    // Run the Ball_test_movement test
    printf("Testing Laser_test_movement...\n");
    bool test_passed3 = Laser_test_movement();

    // Print out the result of this test
    if (test_passed3) {
        printf("...Passed!\n");
    }
    else {
        printf("...FAILED!\n");
        ++n_tests_failed; // Increment number of failures
    }

    // Finish by printing a summary of the tests
    if (n_tests_failed > 0) {
        printf("%d tests FAILED!\n", n_tests_failed);
    }
    else {
        printf("All tests passed!\n");
    }

    return n_tests_failed;
}

#endif