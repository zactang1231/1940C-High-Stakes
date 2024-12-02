#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h"
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
#include "robot-config.h"
#include "lady-brown.h"

#include <iostream>

void autocontrolLoop(void* param) {
    controller1.clear();
    leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    leftMotors.set_current_limit(2500);
    rightMotors.set_current_limit(2500);
    uptake.set_current_limit(2500);
    preroller.set_current_limit(2500);

    leftMotors.set_reversed(true);
    rightMotors.set_reversed(false);

    // leftMotors.move_velocity(200);
    // rightMotors.move_velocity(200);

    preroller.move_velocity(200);
    uptake.move_velocity(200);
    
    preroller.set_brake_mode (pros::E_MOTOR_BRAKE_COAST);
    uptake.set_brake_mode (pros::E_MOTOR_BRAKE_COAST);
    lb.set_brake_mode (pros::E_MOTOR_BRAKE_HOLD);

    reject.set_led_pwm(100);
    
    lb.tare_position();

    // this runs during auton the whole time
    while (true) {
        rejectRing();
        LBSpinToTarget();

        // delay so the Brain doesn't explode
        pros::delay(25);
    }
}