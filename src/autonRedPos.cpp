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
// void printTask()
// {
//     while(true)
//     {
//         std::string outtext = "X: " + std::to_string(chassis.getPose().x) + "Y: " + std::to_string(chassis.getPose().y);
//         pros::lcd::set_text(4, outtext);
//         pros::delay(200);
//     }
// }
void redPos() {
    controller1.rumble("...");
    curState = 1;
    chassis.setPose(-51, -60, 90);
    chassis.moveToPoint(-16.8, -60, 3000);
    // chassis.turnToHeading(45, 2000);
    chassis.moveToPose(-10.5, -56, 55, 2000);
    pros::delay(1000);
    upState();
    upState();
    upState();
    pros::delay(2000);
    
    downState();
    chassis.moveToPose(12,-57.5,55,2000);
    chassis.turnToHeading(180, 2000);
    chassis.turnToHeading(0, 2000);
    downState();
    downState();
    downState();
    // chassis.moveToPoint(-37, -60, 3000);
    intake.move(127);
    chassis.moveToPose(-28, -44.5, 180, 3000);
    // chassis.turnToHeading(180, 2000);
    chassis.moveToPoint(-24.5, -24, 3000, {.forwards = false});
    pros::delay(1000);
    mogo.set_value(true);
    pros::delay(1000000000);
    chassis.moveToPoint(-11, -13, 3000);
}

