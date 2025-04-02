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
#include <string>
#include <type_traits>

#include <iostream>

// Enum to represent the different lift states
// enum class LiftState { DEFAULT, LOADING_2, LOADING_1, SCORING, FLIPPING };

// Array of target positions corresponding to each state (in degrees)
const double states[5] = {0.0, 15.0, 90.0, 135.0, 180.0}; //change pls

int curState = 0;
int target = 0;

void upState() {
    curState += 1;
    target = states[curState];
}

void downState() {
    curState -= 1;
    target = states[curState];
}

void lbControl() {
    double kP = 0.1;
    double kD = 0.1;
    double prevError = 0;
    double error = target - lb_sensor.get_position();

    double proportional = kP * error;
    double derivative = (kD * (error - prevError)) / 20; // divide by time delay
    double output = proportional + derivative;
    lb.move(output);

    prevError = error;

    pros::delay(20);
}

