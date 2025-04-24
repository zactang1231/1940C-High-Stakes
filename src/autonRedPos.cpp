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
void printTask()
{
    while(true)
    {
        std::string outtext = "X: " + std::to_string(chassis.getPose().x) + "Y: " + std::to_string(chassis.getPose().y);
        pros::lcd::set_text(4, outtext);
        pros::delay(200);
    }
}
void redPos() {
    chassis.setPose(-51, -60, 90);
    chassis.moveToPoint(-16.7, -60, 3000);
    // chassis.turnToHeading(45, 2000);
    chassis.moveToPose(-10, -55.5, 45, 2000);
    chassis.turnToHeading(315, 2000);

    // chassis.moveToPoint(-37, -60, 3000);

    // pros::delay(100000000);
    chassis.moveToPose(-24, -47.5, 45, 3000);
    chassis.turnToHeading(180, 2000);
    chassis.moveToPoint(-24.5, -24, 3000);
    chassis.moveToPoint(-11, -13, 3000);

    pros::delay(100000000);
}

