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
ASSET(redneg1_txt);
ASSET(redneg2_txt);

void redneg() {
    preroller.move(127);
    uptake_mutex.take();
    intake.move(0);
    handleLBStateUp();
    mogo.set_value(true);

    chassis.setPose(-56, 14.5, 0);

    // First path
    chassis.moveToPose(-56, 43.5, 0, 4000);
    chassis.moveToPose(-23.5, 23.5, 120, 4000, {.forwards = false, .maxSpeed = 50});
    // chassis.follow(blueneg1_txt, 15, 10000, false);
    pros::delay(2000);
    mogo.set_value(false);
    // Ring
    chassis.moveToPose(-23.5, 47, 0, 4000, {.maxSpeed = 127});
    intake.move(127);
    // Quad stack
    chassis.moveToPose(-10, 49, 80, 4000);
    chassis.moveToPose(-23.5, 47, 80, 4000, {.forwards = false});
    chassis.turnToHeading(100, 4000);
    chassis.moveToPose(-10, 44.5, 100, 4000);
    chassis.moveToPose(-23.5, 47, 100, 4000);
    // Ladder
    chassis.moveToPose(-15, 14, 135, 4000, {.forwards = false});
    handleLBStateDown();
    uptake_mutex.give();
}   