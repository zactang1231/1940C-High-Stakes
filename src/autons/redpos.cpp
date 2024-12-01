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
    preroller.move(127);
    uptake.move(127);
    handleLBStateUp();
    mogo.set_value(false);

    chassis.setPose(-61, -33, 90);

    // First path
    chassis.follow(redpos1_txt, 15, 10000, false);
    mogo.set_value(true);
    // Rings
    chassis.moveToPose(-23.5, -23.5, 180, 4000);
    chassis.moveToPose(-23.5, -47, 180, 4000);
    // Mogo
    chassis.moveToPose(-13.6, -55, 270, 4000);
    doinker.set_value(true);
    chassis.moveToPose(-35, -55, 270, 4000, {.forwards = false});
}   