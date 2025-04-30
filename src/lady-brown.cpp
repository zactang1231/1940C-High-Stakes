#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h"
#include "pros/rtos.h"
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
const double states[5] = {0.0, 3400.0, 4800.0, 15000.0, 25000.0}; //change pls

int curState;
double target = states[0];
double previous_error = 0;

void upState() {
    if (curState < 5) {
        curState += 1;
        target = states[curState];
    }
}

void downState() {
    if (curState > 0) {
        curState -= 1;
        target = states[curState];
    }
}
 
void lbControl() {
    lb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    if (controller1.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if (controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            lb.move(127);
        } else if (controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            lb.move(-127);
        } else {
            lb.move(0);
        }
        previous_error = target - lb_sensor.get_position();
      } 
    else {
    
        target = states[curState];

        // Gains
        double kP = 0.02;
        double kD = 0.0005;

        double dt = 0.02; // 20 ms time delay

        double error = target - lb_sensor.get_position();

        double derivative = (error - previous_error) / dt;

        double output = kP * error + kD * derivative;

        if (fabs(error) > 10) {
            lb.move(output);
        }
        // lb.move(output);

        previous_error = error;

        pros::delay(20);
    }
}

