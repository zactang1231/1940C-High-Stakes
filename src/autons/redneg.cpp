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
    uptake.move(127);
    handleLBStateUp();
    mogo.set_value(false);

    chassis.setPose(-50.5, 14.5, 90, 4000);

    // First path
    chassis.moveToPose(-47, 23.5, 90, 4000);
    chassis.follow(skillsauto1_txt, 15, 10000, {.forwards = false});
    mogo.set_value(true);
    // Second path
    chassis.moveToPose(-35, 32, 270, 4000);
    chassis.follow(skillsauto2_txt, 15, 10000);
    // Rings
    chassis.moveToPose(-23, 47, 100, 4000, {.forwards = false});
    chassis.moveToPose(-10, 45, 100, 4000);
    // Ladder
    chassis.moveToPose(-14, 9.5, 315, 4000, {.forwards = false});
    handleLBStateDown();
}   