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

#include <iostream>

bool mogoActivated = false;
bool doinkerActivated = false;
bool intake2Activated = false;

enum UptakeState { OFF, FORWARD, REVERSE };
UptakeState uptakeState = OFF;

bool previousB = false;
bool previousX = false;
bool previousY = false;
bool previousL1 = false;

bool rejectingOn = true;

float trackWidth = 13.0;
float wheelDiameter = 4.0; 

float horizontalOffset = -7;
float verticalOffset = -5.4;

// controller
pros::Controller controller1(pros::E_CONTROLLER_MASTER);

// drivetrain motors

// pros::Motor L1(-11, pros::MotorGearset::green);
// pros::Motor L2(-14, pros::MotorGearset::green);
// pros::Motor L3(-16, pros::MotorGearset::green);

// pros::Motor R1(12, pros::MotorGearset::green);
// pros::Motor R2(13, pros::MotorGearset::green);
// pros::Motor R3(5, pros::MotorGearset::green);

// pros::MotorGroup leftMotors({L1, L2, L3}); // left motor group - ports 11, 14, 16
// pros::MotorGroup rightMotors({R1, R2, R3}); // left motor group - ports 11, 14, 16

pros::MotorGroup leftMotors({-11, -14, -16}, pros::MotorGearset::green); // left motor group - ports 11, 14, 16
pros::MotorGroup rightMotors({12, 13, 5}, pros::MotorGearset::green); // right motor group - ports 12, 13, 5 (all reversed, so they're negative)

// motors - migrated from robot-config.cpp from states code
pros::Motor preroller(-1, pros::MotorGearset::green);
pros::MotorGroup uptake({3,15}, pros::MotorGearset::green);

// pistons - migrated from robot-config.cpp from states code
pros::adi::DigitalOut mogo('A');
pros::adi::DigitalOut doinker('B');

// sensors - migrated from robot-config.cpp from states code
pros::Imu inertial(7);
pros::Optical reject(4);

/* TRACKING WHEELS */

// tracking wheels. we are using rotation sensors with 2" wheels
pros::Rotation horizontalEnc(18);
pros::Rotation verticalEnc(6);
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, horizontalOffset); // tune
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, verticalOffset); // tune

/* LEMLIB DRIVETRAIN */

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              12.9921,
                              lemlib::Omniwheel::NEW_4, // we're using new 4" omnis (omni gen 2)
                              350, // drivetrain rpm is 350 (84/48*200)
                              8 // according to lemlib, centre traction wheels means horizontal drift is approx 8. we'll probs tune this later
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel left
                            nullptr, // vertical tracking wheel right
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &inertial // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

pros::Mutex uptake_mutex;

void prerollerForward() {
    preroller.move(127);
}

void prerollerReverse() {
    preroller.move(-127);
}

void prerollerStop() {
    preroller.move(0);
}

void handleUptakeState() {
        bool currentL1 = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool currentL2 = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    // Toggle forward state with L1
    if (currentL1 && !previousL1) { // Rising edge detection for L1
        if (uptakeState == FORWARD) {
            uptakeState = OFF; // Stop if already running forward
        } else {
            uptakeState = FORWARD; // Start running forward
        }
    }

    // Hold reverse state with L2
    if (currentL2) { // L2 is held down
        uptakeState = REVERSE; // Run reverse while holding L2
    } else if (uptakeState == REVERSE) {
        uptakeState = OFF; // Stop reverse when L2 is released
    }

    // Update previous states
    previousL1 = currentL1;
}

void updateUptakeMotor() {
    uptake_mutex.take(); // lock mutex (uptake is mine now)

    switch (uptakeState) {
        case FORWARD:
            uptake.move(127);
            std::cout << "Uptake: FORWARD" << std::endl;
            break;
        case REVERSE:
            uptake.move(-127);
            std::cout << "Uptake: REVERSE" << std::endl;
            break;
        case OFF:
        default:
            uptake.move(0);
            std::cout << "Uptake: OFF" << std::endl;
            break;
    }

    uptake_mutex.give(); // release mutex (uptake can be used elsewhere)
}

void uptakeForward() {
    uptake_mutex.take(); // lock mutex (uptake is mine now)

    bool currentL1 = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    if (currentL1 && !previousL1) { // Rising edge detection for L1
        if (uptakeState == FORWARD) {
            uptakeState = OFF; // Stop the motor if already running forward
        } else {
            uptakeState = FORWARD; // Start moving forward
        }
    }
    previousL1 = currentL1; // Update the button state

    uptake_mutex.give(); // lock mutex (uptake is mine now)
}

void uptakeReverse() {
    uptake_mutex.take(); // lock mutex (uptake is mine now)

    uptakeState = REVERSE;

    uptake_mutex.give(); // lock mutex (uptake is mine now)
    // bool currentL2 = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    // if (currentL2 && !previousL2) { // Rising edge detection for L2
    //     if (uptakeState == REVERSE) {
    //         uptakeState = OFF; // Stop the motor if already running reverse
    //     } else {
    //         uptakeState = REVERSE; // Start moving reverse
    //     }
    // }
    // previousL2 = currentL2; // Update the button state
}

// void uptakeForward() {
//     if (intake2Activated) {
//         intake2Activated = false;
//         uptake.move(0);
//     } else {
//         intake2Activated = true;
//         uptake.move(127);
//     }
// }

// void uptakeForward() {
//     uptake.move(127);
// }

void mogoButton() {
    if (mogoActivated) {
        mogoActivated = false;
        mogo.set_value(false);
    } else {
        mogoActivated = true;
        mogo.set_value(true);
    }
}

void doinkerButton() {
    if (doinkerActivated) {
        doinkerActivated = false;
        doinker.set_value(false);
    } else {
        doinkerActivated = true;
        doinker.set_value(true);
    }
}

void rejectRing(char acceptColour) {
    if (rejectingOn) {
        // Colour sorting
        pros::c::optical_rgb_s_t rgb = reject.get_rgb();
        int red = rgb.red;     // get red 
        int green = rgb.green; // get green 
        int blue = rgb.blue;   // get blue

        // Define thresholds for red and blue detection
        const int RED_THRESHOLD = 150;
        const int BLUE_THRESHOLD = 150;

        if (acceptColour == 'b') {
            if (red > RED_THRESHOLD && green < 100 && blue < 100) {
                uptake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                pros::delay(50);
                uptake.move(0);
                pros::delay(20);
                uptake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                uptake.move(127);
                controller1.clear_line(1);
                controller1.set_text(1,0, "bye bye red ring");
                pros::delay(500);
            }
        }
        else if (acceptColour == 'r') {
            if (blue > BLUE_THRESHOLD && red < 100 && green < 100) {
                uptake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                pros::delay(50);
                uptake.move(0);
                pros::delay(20);
                uptake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                uptake.move(127);
                controller1.clear();
                controller1.set_text(1,0, "bye bye blue ring");
            }
        }
    }
}

void printRejectStatus() {
    controller1.clear();
    if (rejectingOn) {
        controller1.print(1, 1, "Rejecting is ON.");
    } else {
        controller1.print(1, 1, "Rejecting is OFF.");
    }
}

void rejectOnOff() {
    if (rejectingOn) {
        rejectingOn = false;
        printRejectStatus();
    } else {
        rejectingOn = true;
        printRejectStatus();
    }
}

void turnToAngle(double targetAngleDegrees, int maxSpeed) {
 // Convert the target angle to radians
    double targetAngleRadians = targetAngleDegrees * M_PI / 180.0;

    // Reset tracking wheel
    horizontal.reset();
    vertical.reset();

    // Start turning motors
    leftMotors.move_velocity(maxSpeed);  // Opposite directions for turning
    rightMotors.move_velocity(-maxSpeed);

    // Monitor the turn
    while (true) {
        // Get the distance traveled by the horizontal tracking wheel
        double horizontalWheelDistance = horizontal.getDistanceTraveled();
        // double horizontalWheelDistance = horizontalEnc.get_position() / 360.0 * (2.0 * M_PI * 2.0); // Assume 2" wheel diameter

        // Calculate the current angle turned
        double currentAngleRadians = horizontalWheelDistance / horizontalOffset;

        // Break if the target angle is reached
        if (fabs(currentAngleRadians - targetAngleRadians) < 0.05) { // Threshold in radians
            break;
        }

        // Log for debugging
        std::cout << "Target Angle: " << targetAngleRadians
                  << " | Current Angle: " << currentAngleRadians
                  << " | Horizontal Distance: " << horizontalWheelDistance << std::endl;

        pros::delay(10);
    }

    // Stop motors
    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);
}

void moveDistance(double targetDistanceInches, int maxSpeed) {
    // Reset tracking wheel
    horizontal.reset();
    vertical.reset();

    // Start motors
    leftMotors.move_velocity(maxSpeed);
    rightMotors.move_velocity(maxSpeed);

    // Monitor the movement
    while (true) {
        // Get the distance traveled from the vertical tracking wheel
        double currentDistance = vertical.getDistanceTraveled();

        // Break if the target distance is reached
        if (fabs(currentDistance) >= fabs(targetDistanceInches)) {
            break;
        }

        // Log for debugging
        std::cout << "Target Distance: " << targetDistanceInches
                  << " | Current Distance: " << currentDistance
                  << std::endl;

        pros::delay(10);
    }

    // Stop motors
    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);
}
