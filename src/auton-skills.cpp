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
ASSET(skillsauto1_txt);
ASSET(skillsauto2_txt);
ASSET(skillsauto3_txt);
ASSET(skillsauto4_txt);

void autonSkills() {
    preroller.move(127);
    uptake.move(127);
    handleLBStateUp();
    mogo.set_value(false);

    chassis.setPose(-60, 0, 90, 4000);

    // --- Q1 --- //

    // Go get mogo
    chassis.moveToPose(-34, 17.5, 60, 4000,  {.forwards = false});
    mogo.set_value(true);
    // First path
    chassis.moveToPose(-47, 23.5, 90, 4000);
    chassis.follow(skillsauto1_txt, 15, 10000);
    // Second path
    chassis.moveToPose(0, 57, 220, 4000);
    chassis.follow(skillsauto2_txt, 15, 10000);
    // Q1 Rings
    chassis.moveToPose(-47, 47, 0, 4000, {.forwards = false});
    chassis.moveToPose(-47, 59, 0, 4000);
    // Q1 mogo
    chassis.moveToPose(-47, 56, 0, 4000, {.forwards = false});
    chassis.moveToPose(-60, 60, 135, 4000, {.forwards = false});
    mogo.set_value(false);


    // --- Q2 --- //

    // Go get mogo
    chassis.moveToPose(-37, -17.5, 240, 4000,  {.forwards = false});
    mogo.set_value(true);
    // First path
    chassis.moveToPose(-47, -23.5, 90, 4000);
    chassis.follow(skillsauto3_txt, 15, 10000);
    // Second path
    chassis.moveToPose(0, -57, 320, 4000);
    chassis.follow(skillsauto4_txt, 15, 10000);
    // Q2 Rings
    chassis.moveToPose(-47, -47, 180, 4000, {.forwards = false});
    chassis.moveToPose(-47, -59, 0, 4000);
    // Q2 mogo
    chassis.moveToPose(-47, -56, 0, 4000, {.forwards = false});
    chassis.moveToPose(-60, -60, 135, 4000, {.forwards = false});
    mogo.set_value(false);
}   