#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h"
#include "opcontrol.h"
#include "autocontrol.h"
#include "lady-brown.h"
#include "new-lb-control.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.h"
#include "pros/rotation.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include <cstddef>
#include <string>
#include <type_traits>

#include <iostream>

// Resource states (bools)

bool mogoActivated = false;
bool doinkerActivated = false;
bool intake2Activated = false;

// Resource states (enums)

enum UptakeState { OFF, FORWARD, REVERSE };
UptakeState uptakeState = OFF;

// Button debouncers

bool previousB = false;
bool previousX = false;
bool previousY = false;
bool previousL1 = false;

bool rejectingOn = true;

float trackWidth = 13.0;
float wheelDiameter = 3.0; 

float horizontalOffset = -5.02;
float verticalOffset = -0.75;

char acceptColour;

// controller
pros::Controller controller1(pros::E_CONTROLLER_MASTER);

// drivetrain motors

// pros::Motor L1(-11, pros::MotorGearset::green);
// pros::Motor L2(-14, pros::MotorGearset::green);
// pros::Motor L3(-16, pros::MotorGearset::green);x

// pros::Motor R1(12, pros::MotorGearset::green);
// pros::Motor R2(13, pros::MotorGearset::green);
// pros::Motor R3(5, pros::MotorGearset::green);

// pros::MotorGroup leftMotors({L1, L2, L3}); // left motor group - ports 11, 14, 16
// pros::MotorGroup rightMotors({R19, R2, R3}); // left motor group - ports 11, 14, 16

pros::MotorGroup leftMotors({1, -2, -3}, pros::MotorGearset::blue); // left motor group - ports 11, 14, 16
pros::MotorGroup rightMotors({-4, 5, 6}, pros::MotorGearset::blue); // right motor group - ports 12, 13, 5 (all reversed, so they're negative)

// motors - migrated from robot-config.cpp from states code
pros::Motor intake(9, pros::MotorGearset::blue);
pros::Motor preroller(20, pros::MotorGearset::blue);
pros::MotorGroup lb({10,-11}, pros::MotorGearset::green);
pros::Rotation lb_sensor(-20);

// pistons - migrated from robot-config.cpp from states code
pros::adi::DigitalOut mogo('A');
pros::adi::DigitalOut doinker('B');

// sensors - migrated from robot-config.cpp from states code
pros::Imu inertial(7);
pros::Optical reject(4);

/* TRACKING WHEELS */

// tracking wheels. we are using rotation sensors with 2" wheels
pros::Rotation horizontalEnc(-8);
pros::Rotation verticalEnc(-7);
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, horizontalOffset);
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, verticalOffset);

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
lemlib::ControllerSettings linearController(7, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            25, // derivative gain (kD)
                                            0, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            35 // maximum acceleration (slew)
);

// 7, 0, 20

// angular motion controller
lemlib::ControllerSettings angularController(4, // proportional gain (kP)
                                             0, // integral gain (kI)1
                                             40, // derivative gain (kD)
                                             0, // anti windup
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

// Mutex inits

pros::Mutex uptake_mutex;

// Preroller moves

void prerollerForward() {
    preroller.move(-127);
}

void prerollerReverse() {
    preroller.move(127);
}

void prerollerStop() {
    preroller.move(0);
}

// LB moves

// void LBForward() {
//     lb.move(-127);
// }

// void LBReverse() {
//     lb.move(127);controller1.clear();
// }

// void LBStop() {
//     lb.move(0);
// }

void handleUptakeState() {
    bool currentL1 = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool currentL2 = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

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
            intake.move(127);
            std::cout << "Uptake: FORWARD" << std::endl;
            break;
        case REVERSE:
            intake.move(-127);
            std::cout << "Uptake: REVERSE" << std::endl;
            break;
        case OFF:
        default:
            intake.move(0);
            std::cout << "Uptake: OFF" << std::endl;
            break;
    }

    uptake_mutex.give(); // release mutex (uptake can be used elsewhere)
}

void uptakeForward() {
    uptake_mutex.take(); // lock mutex (uptake is mine now)

    bool currentL1 = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
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

void rejectRing() {
    if (rejectingOn) {
        // Colour sorting
        pros::c::optical_rgb_s_t rgb = reject.get_rgb();
        int red = rgb.red;     // get red 
        int green = rgb.green; // get green 
        int blue = rgb.blue;   // get blue
        // Define thresholds for red and blue detection
        const int RED_THRESHOLD = 1000;
        const int BLUE_THRESHOLD = 1000;

        if (acceptColour == 'b') {
            if (red > RED_THRESHOLD && green < red && blue < red) {
                uptake_mutex.take();
                intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                pros::delay(25);
                intake.move(0);
                pros::delay(1000);
                intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                intake.move(127);
                controller1.clear();
                controller1.print(0,0, "bye bye red ring");
                pros::delay(500);
                uptake_mutex.give();
            }
        }
        else if (acceptColour == 'r') {
            if (blue > BLUE_THRESHOLD && red < blue && green < blue) {
                uptake_mutex.take();
                intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                pros::delay(25);
                intake.move(0);
                pros::delay(1000);
                intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                intake.move(127);
                controller1.clear();
                controller1.print(0,0, "bye bye blue ring");
                uptake_mutex.give();
            }
        }
    }
}

void rejectOnOff() {
    if (rejectingOn) {
        rejectingOn = false;
        reject.set_led_pwm(0);
    } else {
        rejectingOn = true;
        reject.set_led_pwm(100);
    }
}

void setAllianceColour(char allianceColour) {
    if (allianceColour == 'r') {
        acceptColour = 'r';
    } else if (allianceColour == 'b') {
        acceptColour = 'b';
    }
}

// Raw movement

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

// Initialize robot and start the task
void opcontrolInit() {
    pros::Task opcontrolLoopTask(opcontrolLoop);
    // pros::Task lbLoopTask(LBSpinToTarget);
    pros::Task lift_task([] {
        while (true) {
            if (controller1.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
                downState();
            } else if (controller1.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
                upState();
            }
            lbControl();
        }
    });
}

void autocontrolInit() { 
    pros::Task autocontrolLoopTask(autocontrolLoop);
}