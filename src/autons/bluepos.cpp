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
ASSET(bluepos1_txt);

void bluepos() {
    // --- RUSH --- //

    // Ring starts in LB

    preroller.move(127);
    handleLBStateUp();

    chassis.setPose(61, -33, 270);
    // Mogo
    chassis.moveToPose(43, -33, 295, 4000);
    pros::delay(1000);
    uptake.move(0);

    chassis.moveToPose(23.5, -23.5, 295, 4000);
    chassis.turnToPoint(23.5, -23.5, 2000);
    chassis.moveToPose(23.5, -47, 180, 4000);
    chassis.turnToPoint(56, -47, 2000);
    chassis.moveToPose(56, -47, 90, 4000);
    chassis.turnToPoint(58, -55, 2000);
    chassis.moveToPose(58, -55, 165, 4000);
    // --- NON RUSH --- //

    // preroller.move(127);
    // uptake.move(127);
    // handleLBStateUp();
    // mogo.set_value(false);

    // chassis.setPose(61, -34, 270);

    // // First path
    // // chassis.follow(bluepos1_txt, 15, 10000, false);
    // chassis.moveToPose(42, -34, 300, 4000);
    // chassis.turnToPoint(31.5, -28, 4000);
    // chassis.moveToPose(31.5, -28, 300, 4000);
    // pros::delay(1500);
    // mogo.set_value(true);
    // // Ring
    // chassis.moveToPose(23.5, -47, 180, 4000);
    // // Rush Mogo
    // mogo.set_value(true);
    // chassis.turnToHeading(285, 4000);
    // chassis.moveToPose(15, -44.5, 285, 4000);
    // doinker.set_value(true);
    // chassis.moveToPose(34, -50.5, 285, 4000, {.forwards = false});
    // chassis.moveToPose(15, -44.5, 285, 4000, {.forwards = false});
    // pros::delay(1500);
    // mogo.set_value(false);
}   