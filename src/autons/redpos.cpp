#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h"
#include "robot-config.h"
#include "lady-brown.h"
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

// get a path used for pure pursuit
// this needs to be put outside a function
// '.' is replaced with "_" to make c++ happy
ASSET(redpos1_txt);

void redpos() {

    // --- RUSH --- //

    // Ring starts in LB

    preroller.move(127);
    uptake.move(127);
    handleLBStateUp();
    // mogo.set_value(false);

    chassis.setPose(-61, -47, 90);
    // Ring line
    chassis.moveToPose(-23.5, -47, 90, 4000);
    // chassis.moveToPose(-37.5, -47, 90, 4000, {});

    chassis.moveToPose(-18, -45.5, 75, 4000);
    doinker.set_value(true);
    chassis.moveToPose(-34, -50, 75, 4000, {.forwards = false, .maxSpeed = 25});
    doinker.set_value(false);
    chassis.moveToPose(-15, -44.5, 75, 4000, {.forwards = false});
    pros::delay(1500);
    mogo.set_value(false);
    uptake.move(127);
    pros::delay(2000);
    mogo.set_value(true);
    handleLBStateDown();
    uptake.move_relative(-200, 127);
    chassis.moveToPoint(-23.5, -47, 4000);
    chassis.turnToHeading(180, 4000);
    chassis.moveToPose(-23.5, -34, 0, 4000, {.forwards = false});
    pros::delay(1500);
    mogo.set_value(false);
    uptake.move(127);

    // --- NON RUSH --- //

    // preroller.move(127);
    // uptake.move(127);
    // handleLBStateUp();
    // mogo.set_value(false);

    // chassis.setPose(61, -34, 270);

    // // First path
    // // chassis.follow(bluepos1_txt, 15, 10000, false);
    // chassis.moveToPose(-42, -34, 60, 4000);
    // chassis.turnToPoint(-31.5, -28, 4000);
    // chassis.moveToPose(-31.5, -28, 60, 4000);
    // pros::delay(1500);
    // mogo.set_value(true);
    // // Ring
    // chassis.moveToPose(-23.5, -47, 180, 4000);
    // // Rush Mogo
    // mogo.set_value(true);
    // chassis.turnToHeading(75, 4000);
    // chassis.moveToPose(-15, -44.5, 75, 4000);
    // doinker.set_value(true);
    // chassis.moveToPose(-34, -50.5, 75, 4000, {.forwards = false});
    // chassis.moveToPose(-15, -44.5, 75, 4000, {.forwards = false});
    // pros::delay(1500);
    // mogo.set_value(false);
}   