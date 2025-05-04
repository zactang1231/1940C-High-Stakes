#include "lemlib/chassis/chassis.hpp"
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
#include "../include/lady-brown.h"

#include <iostream>

void blueNeg() {
    leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    lb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    curState = 0;
    chassis.setPose(-63, 37, 90);

    intake.move(-127);
    chassis.moveToPoint(-23.5, 37, 2000, {.maxSpeed = 100});

    chassis.moveToPoint(-12.5, 38.75, 4000, {.maxSpeed=60});

    chassis.moveToPoint(-23.5, 37, 3000, {.forwards=false});
    pros::delay(800);
    intake.move(0);

    chassis.turnToHeading(0, 1000);
    doinkerButton();
    chassis.moveToPoint(-13, 14, 4000, {.forwards = false, .maxSpeed = 80});
    // chassis.moveToPose(-23.5, 23.5, 0, 4000, {.forwards=false});
    // chassis.moveToPose(-23.5, 20, 0, 4000, {.forwards=false});
    // chassis.moveToPose(-13, 14, 60, 1500, {.forwards=false});
    // chassis.moveToPoint(-10, 21, 1500, {.forwards=false});
    // chassis.moveToPoint(-13, 23.5, 1500, {.forwards=false});

    pros::delay(1500);

    mogo.set_value(true);
    intake.move(-127);

    pros::delay(500);

    chassis.moveToPoint(-10, 50, 800);

    chassis.turnToPoint(-34, 42, 800);
    chassis.moveToPoint(-34, 42, 4000);

    leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    lb.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(100000000);

    // intake.move(-127);
    // doinkerButton();
    // chassis.moveToPose(-23.5, 37, 90, 3000, {.forwards=false, .maxSpeed = 80});
    // chassis.moveToPoint(-23.5, 20, 3000, {.forwards = false});
    // pros::delay(1500);
    // mogo.set_value(true);

    // chassis.turnToPoint(-23.5, 45, 4000);
    // chassis.moveToPose(-23.5, 45, 0, 8000, {.lead = 0});

    // leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // lb.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // pros::delay(100000000);
}