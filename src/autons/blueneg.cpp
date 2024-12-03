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
ASSET(blueneg1_txt);
ASSET(blueneg2_txt);

void blueneg() {
    preroller.move(127);
    uptake.move(127);
    handleLBStateUp();
    mogo.set_value(true);

    chassis.setPose(56, 14.5, 90);

    // First path
    chassis.moveToPose(56, 43.5, 240, 4000, {.forwards = false});
    chassis.moveToPose(33.5, 29.5, 240, 4000, {.forwards = false});
    pros::delay(1500);
    // chassis.follow(blueneg1_txt, 15, 10000, false);
    mogo.set_value(false);
    // Second path
    chassis.moveToPose(35, 32, 270, 4000);
    chassis.follow(blueneg2_txt, 15, 10000);
    // Rings
    chassis.moveToPose(23, 47, 100, 4000, {.forwards = false});
    chassis.moveToPose(10, 45, 100, 4000);
    // Ladder
    chassis.moveToPose(14, 9.5, 315, 4000, {.forwards = false});
    handleLBStateDown();
}   