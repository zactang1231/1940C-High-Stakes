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
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    // --- RUSH --- //

    // Ring starts in LB

    preroller.move(127);

    // Set initial pose, invert x and adjust theta
    chassis.setPose(0, 0, 152.8);  // Assuming angles are in degrees

    mogo.set_value(true);

    // Move to mirrored pose
    chassis.moveToPose(-12.5, 26.4, 152.8, 1800, {.forwards=false, .lead=0, .maxSpeed = 105, .minSpeed = 15}, true);

    pros::delay(700);

    mogo.set_value(false); // camp doww

    // Turn to mirrored heading
    chassis.turnToHeading(223.64, 1800);  // Mirror angle

    intake.move(127);

    chassis.moveToPose(-22.5, 10.62, 223.64, 2500, {.forwards=true, .lead=0, .maxSpeed = 100, .minSpeed = 25}, true);

    pros::delay(700);

    chassis.turnToHeading(0, 2000);


    // // Move to another mirrored pose
    // chassis.moveToPose(4.4, 26.4, 270, 2500, {.forwards=true, .lead=0, .maxSpeed = 115, .minSpeed = 15}, false);

    // pros::delay(1000);

    // chassis.turnToHeading(270, 2500);

    // // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);




    // // --- RUSH --- //

    // // Ring starts in LB

    // preroller.move(127);

    // chassis.setPose(0, 0, 152.8);

    // mogo.set_value(true);

    // chassis.moveToPose(-12.5, 26.4, 152.8, 1800, {.forwards=false, .lead=0, .maxSpeed = 105, .minSpeed = 15}, false);

    // mogo.set_value(false); // camp doww

    // pros::delay(300);

    // uptake.move(127);

    // chassis.turnToHeading(90, 1800);

    // chassis.moveToPose(4.4, 26.4, 94.08, 2500, {.forwards=true, .lead=0, .maxSpeed = 115, .minSpeed = 15}, false);

    // pros::delay(1000);

    // chassis.turnToHeading(0, 2500);

    //152.8
    // Mogo
    // chassis.moveToPose(43, -33, 295, 4000);
    // pros::delay(1000);
    // uptake.move(0);

    // chassis.moveToPose(23.5, -23.5, 295, 4000);
    // chassis.turnToPoint(23.5, -23.5, 2000);
    // chassis.moveToPose(23.5, -47, 180, 4000);
    // chassis.turnToPoint(56, -47, 2000);
    // chassis.moveToPose(56, -47, 90, 4000);
    // chassis.turnToPoint(58, -55, 2000);
    // chassis.moveToPose(58, -55, 165, 4000);
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