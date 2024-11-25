#include "main.h"
// #include "chassis.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"

#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "pros/screen.h"
#include "robot-config.h"
#include "opcontrol.h"

/**pros c fetch <template>
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::screen::print(TEXT_MEDIUM, 0, "Yay! The code works, the bot has been initialised. Good job for now.");
            pros::screen::print(TEXT_MEDIUM, 1, "X: %f", chassis.getPose().x); // x
            pros::screen::print(TEXT_MEDIUM, 2, "Y: %f", chassis.getPose().y); // y
            pros::screen::print(TEXT_MEDIUM, 3, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
// ASSET(example_txt); // '.' replaced with "_" to make c++ happy

/**
 * Runs during auto
 */
void autonomous() {
    setAllianceColour('r');
    chassis.setPose(0,0,0);

    leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // chassis.turnToHeading(
    // 270,
    // 4000,
    // {.maxSpeed = 60}, // will never exceed 120
    // false // this motion will not block execution
    // ); 

    turnToAngle(90, 70);
}

/**
 * Runs in driver control
 */
void opcontrol() {
    setAllianceColour('r');
    robotInit();
    leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}
