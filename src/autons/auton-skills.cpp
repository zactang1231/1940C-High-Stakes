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
ASSET(skillsauto0_txt);
ASSET(skillsauto1_txt);
ASSET(skillsauto2_txt);
ASSET(skillsauto3_txt);
ASSET(skillsauto4_txt);
ASSET(skillsauto5_txt);

void autonSkills() {
    uptake_mutex.take();
    mogo.set_value(true);
    // mogo.set_value(false);

    chassis.setPose(-60, 0, 90);

    // --- Q1 --- //

    // Go get mogo
    chassis.moveToPoint(-47, 0, 2000);
    chassis.turnToPoint(-47, 25, 2000, {.forwards = false});
    chassis.moveToPoint(-47, 17, 2500,  {.forwards = false});
    preroller.move(127);
    pros::delay(2000);
    mogo.set_value(false);
    pros::delay(500);
    // First path
    intake.move(127);
    chassis.moveToPose(-47, 23.5, 90, 2000);
    chassis.follow(skillsauto1_txt, 15, 10000);
    // Q1 Line
    chassis.moveToPose(0, 50, 270, 2500);
    chassis.moveToPose(-61, 50, 270, 4000);
    // Q1 L-Ring
    chassis.moveToPose(-47, 47, 0, 2000, {.forwards = false});
    chassis.moveToPose(-47, 65, 0, 2000);
    // Q1 mogo
    chassis.moveToPose(-47, 56, 0, 2000, {.forwards = false});
    chassis.moveToPose(-60, 60, 135, 2000, {.forwards = false});
    pros::delay(1800);
    intake.move(-127);
    mogo.set_value(true);

    // --- Q2 --- //

    // Go get mogo
    chassis.moveToPose(-45, 47, 0, 4000, {.forwards = false});
    chassis.moveToPoint(-45, 0, 4000, {.forwards = false});
    chassis.turnToPoint(-45, -25, 4000, {.forwards = false});
    chassis.turnToHeading(0, 4000);
    chassis.moveToPoint(-45, -17, 4000,  {.forwards = false});
    preroller.move(127);
    pros::delay(2000);
    mogo.set_value(false);
    pros::delay(500);
    // First path
    intake.move(127);
    chassis.moveToPose(-47, -23.5, 90, 3000);
    chassis.follow(skillsauto3_txt, 15, 10000);
    // Q1 Line
    chassis.moveToPose(0, -50, 270, 2000);
    chassis.moveToPose(-61, -50, 270, 2000);
    // Q1 L-Ring
    chassis.moveToPose(-47, -47, 0, 2000, {.forwards = false});
    chassis.moveToPose(-47, -65, 0, 2000);
    // Q1 mogo
    chassis.moveToPose(-47, -56, 0, 2000, {.forwards = false});
    chassis.moveToPose(-60, -60, 45, 2000, {.forwards = false});
    pros::delay(1800);
    intake.move(-127);
    mogo.set_value(true);

    // --- Q3 --- //

    // First path
    chassis.moveToPose(-52, -52, 0, 4000);
    intake.move(0);
    chassis.follow(skillsauto5_txt, 15, 10000);
    // Go get mogo
    chassis.moveToPose(47, -12, 0, 4000,  {.forwards = false});
    mogo.set_value(true);
    // Q3 Rings
    intake.move(127);
    chassis.moveToPose(47.5, 0, 315, 4000);
    chassis.moveToPose(23.5, 23.5, 315, 4000);
    chassis.moveToPose(47.5, 0, 315, 4000, {.forwards = false});
    chassis.moveToPose(23.5, -23.5, 225, 4000);
    chassis.moveToPose(23.5, -47, 180, 4000);
    chassis.moveToPose(59, -47, 90, 4000);
    chassis.moveToPose(47, -47, 180, 4000, {.forwards = false});
    chassis.moveToPose(47, -59, 180, 4000);
    // Q3 mogo
    chassis.moveToPose(47, -56, 0, 4000, {.forwards = false});
    chassis.moveToPose(60, -60, 135, 4000, {.forwards = false});
    mogo.set_value(false);

    // --- Q4 --- //

    // Go get mogo
    chassis.moveToPose(59, 11.5, 0, 4000, {.forwards = false});
    // Q4 mogo
    chassis.moveToPose(47, -47, 0, 4000, {.forwards = false});
    chassis.moveToPose(60, -60, 135, 4000, {.forwards = false});
    mogo.set_value(false);

    chassis.moveToPoint(42, 42, 4000);
    uptake_mutex.give();
}   