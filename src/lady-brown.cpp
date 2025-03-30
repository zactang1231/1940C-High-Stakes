#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h"
#include "robot-config.h"
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

enum lbState { LOADING, DEFAULT, SCORING };
lbState LBState = LOADING;

bool previousUp = false;
bool previousDown = false;

float LBTargetPos = 0;

pros::Mutex lb_mutex;

// PID constants
double kP = 0.7; // Proportional gain
double kI = 0.0; // Integral gain
double kD = 0.6; // Derivative gain

void LBMoveToTarget(double targetPosition) {
    static double error = 0;               // Difference between target and current position
    static double lastError = 0;           // Previous error for derivative calculation
    static double integral = 0;            // Integral of errors
    static const double tolerance = 5;     // Tolerance for stopping, in degrees
    static const double maxSpeed = 200;    // Maximum motor speed
    double motorSpeed = 0;                 // Calculated motor speed

    // Get the current position
    double currentPosition = lb.get_position();

    // Calculate error
    error = targetPosition - currentPosition;

    // Exit if within tolerance
    if (fabs(error) <= tolerance) {
        lb.move(0); // Stop the motor
        return;     // Exit the function
    }

    // Calculate integral (accumulated error)
    integral += error;
    integral = fmax(fmin(integral, 100), -100); // Anti-windup (limit integral)

    // Calculate derivative (change in error)
    double derivative = error - lastError;

    // Calculate motor speed using PID formula
    motorSpeed = (kP * error) + (kI * integral) + (kD * derivative);

    // Clamp motor speed to maximum limits
    motorSpeed = fmax(fmin(motorSpeed, maxSpeed), -maxSpeed);

    // Set motor speed
    lb.move(motorSpeed);

    // Update last error
    lastError = error;
}

void handleLBStateDown() {
    if (LBState == SCORING) {
        LBState = LOADING; // Go to loading if scoring 
        controller1.print (0,0,"scoring-default");
    } else if (LBState == LOADING) {
        LBState = DEFAULT; // Go to desfault if loading
        controller1.print (0,0,"default-loading");
    } else if (LBState == DEFAULT) {
        // Stay in DEFAULT if already there
        controller1.print (0,0,"loading");
    }
}

void handleLBStateUp() {
    if (LBState == DEFAULT) {
        LBState = LOADING; // Go to loading if default 
        controller1.print (0,0,"loading-default");
    } else if (LBState == LOADING) {
        LBState = SCORING; // Go to scoring if loading
        controller1.print (0,0,"default-scoring");
    } else if (LBState == SCORING) {
        // Stay in SCORING if already there
        controller1.print (0,0,"scoring");
    }
}

void updateLBMotor() {
    lb_mutex.take(); // lock mutex (uptake is mine now)

    switch (LBState) {
        case LOADING:
            // lb.move_absolute(0, 100);
            LBTargetPos = 265;
            std::cout << "Lady Brown: Scoring" << std::endl;
            break;
        case DEFAULT:
            // lb.move_absolute(-210, 100);
            LBTargetPos = 0;
            std::cout << "Lady Brown: Default" << std::endl;
            break;
        case SCORING:
            // lb.move_absolute(-500, 100);
            LBTargetPos = 1000;
            std::cout << "Lady Brown: Loading" << std::endl;
            break;
    }

    lb_mutex.give(); // release mutex (uptake can be used elsewhere)
}

void LBSpinToTarget() {
    while (true) {
        bool currentUp = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
        bool currentDown = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

        if (currentUp && !previousUp) { // Rising edge detection
            handleLBStateUp();
        }
        if (currentDown && !previousDown) { // Rising edge detection
            handleLBStateDown();
        }

        previousUp = currentUp; // Update state
        previousDown = currentDown;

        updateLBMotor();
        LBMoveToTarget(LBTargetPos);
        // lb.move_absolute(LBTargetPos, 100);

        pros::delay(25); // Allow time for button state to settle

    }
}