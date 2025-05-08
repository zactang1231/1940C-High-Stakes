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
        std::string outtext = "X: " + std::to_string(chassis.getPose().x) + "\nY: " + std::to_string(chassis.getPose().y) + "\ntheta: " + std::to_string(chassis.getPose().theta);
        pros::lcd::set_text(4, outtext);
        pros::delay(200);
    }
}
// }
void redPos() {
    leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    lb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    curState = 0;
    // chassis.setPose(-51, -60, 90);
    chassis.setPose(-62,-33,270);
    chassis.moveToPose(-38.772,-32.984,60,2000, {.forwards = false});
    chassis.turnToPoint(-24, -23.5, 2000, {.forwards = false});
    chassis.moveToPoint(-24,-23.5,2000, {.forwards = false, .maxSpeed = 80});
    pros::delay(800);
    mogo.set_value(true);
    pros::delay(800);
    intake.move(-127);
    chassis.moveToPoint(-23.5,-47,1000);
    pros::delay(500);
    chassis.moveToPose(-59,-59,113,4000);
    chassis.moveToPoint(-55,-55,4000, {.forwards = false});
    chassis.moveToPose(-59,-59,113,4000);


    // rush
    // chassis.moveToPoint(-13, -60, 4000, {.maxSpeed = 80}); // true x: -13
    // pros::delay(1600);
    // // upState();
    // // upState();
    // // upState();
    // chassis.turnToHeading(235, 1000);
    // chassis.moveToPoint(-16, -62.5, 4000, {.forwards = false});
    // downState();
    // downState();
    // downState();
    // downState();

    // pros::delay(1000);

    // doinkerPiston.set_value(true);
    // chassis.turnToHeading(32, 700);
    // chassis.moveToPoint(-10, -64, 4000, {.forwards = false});

    // pros::delay(1000);

    // chassis.moveToPose(-51, -60, 90, 4000, {.forwards = false, .maxSpeed = 80});
    // pros::delay(2800);
    // doinkerPiston.set_value(false);
    // chassis.turnToHeading(60, 300);
    // chassis.turnToHeading(120, 300);
    // chassis.turnToHeading(90, 300);
    // intake.move(-127);
    // chassis.turnToPoint(-23.5, -50, 800);
    // chassis.moveToPoint(-23.5, -50, 2000);
    // chassis.turnToPoint(-23.5, -23.5, 800, {.forwards = false});
    // intake.move(0);
    // chassis.moveToPoint(-23.5, -14, 5000, {.forwards = false, .maxSpeed = 40});
    // pros::delay(1500);
    // mogo.set_value(true);
    // intake.move(-127);
    // chassis.moveToPose(-31.6, -30.5, 45,800);





    // upState();
    // upState();
    // upState();
//     //endblock
//     pros::delay(1000000000);
//     leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
//     rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

//     // chassis.turnToHeading(45, 2000);
//     chassis.moveToPose(-10.5, -56, 55, 2000);
//     pros::delay(1000);
//     upState();
//     upState();
//     upState();
//     pros::delay(2000);
    
//     downState();
//     chassis.moveToPose(12,-57.5,55,2000);
//     chassis.turnToHeading(180, 2000);
//     chassis.turnToHeading(0, 2000);
//     downState();
//     downState();
//     downState();
//     // chassis.moveToPoint(-37, -60, 3000);
//     intake.move(127);
//     chassis.moveToPose(-28, -44.5, 180, 3000);
//     // chassis.turnToHeading(180, 2000);
//     chassis.moveToPoint(-24.5, -24, 3000, {.forwards = false});
//     pros::delay(1000);
//     mogo.set_value(true);
//     pros::delay(1000000000);
//     chassis.moveToPoint(-11, -13, 3000);
}

