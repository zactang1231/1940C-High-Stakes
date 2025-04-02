#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h"
#include "robot-config.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.h"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include <cstddef>
#include <cstdlib>
#include <string>
#include <type_traits>

#include <iostream>

// Enum to represent the different lift states
// enum class LiftState { DEFAULT, LOADING_2, LOADING_1, SCORING, FLIPPING };

// Array of target positions corresponding to each state
const double states[5] = {0.0, 3900.0, 4500.0, 13000.0, 25000.0}; //change pls

int curState = 0;
double target = states[0];
double previous_error = 0;

void upState() {
    curState += 1;
    target = states[curState];
    pros::screen::print(TEXT_LARGE,6,"DOWN");
}

void downState() {
    curState -= 1;
    target = states[curState];
    pros::screen::print(TEXT_LARGE,6,"UP");
}
 
void lbControl() {
    // Gains
    double kP = 0.03;
    double kD = 0.001;

    double dt = 0.02; // 20 ms time delay

    double error = target - lb_sensor.get_position();

    double derivative = (error - previous_error) / dt;

    double output = kP * error + kD * derivative;

    if (abs(error) > 10) {
        lb.move(output);
    }
    // lb.move(output);

    previous_error = error;


    pros::delay(20);

    pros::screen::print(TEXT_LARGE,0,"Position: %d", lb_sensor.get_position());
    pros::screen::print(TEXT_LARGE,10,"Target: %d", target);
    pros::screen::print(TEXT_LARGE,5,"Error: %d", error);

}

