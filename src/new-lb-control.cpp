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

pros::Mutex new_lb_mutex;

// --- Constants for LB Presets and Thresholds ---
const double LB_PRESET_HIGH = -840;    // Loading position 1 (higher)
const double LB_PRESET_LOW  = -290;     // Loading position 2 (lower)
const double SNAP_THRESHOLD = 10;       // Allowed deviation to snap (units consistent with lb.get_position())
const int CLICK_THRESHOLD   = 500;       // Time (ms) to distinguish a click from a hold

// Variables to store button press start times and previous states
unsigned long r1PressStart = 0, r2PressStart = 0;
bool previousR1 = false, previousR2 = false;

// PID control function for LB snapping (assumed tuned already)
void NewLBMoveToTarget(double targetPosition) {
    static double lastError = 0;
    static double integral = 0;
    const double tolerance = 5;     // Acceptable error threshold
    const double maxSpeed  = 200;    // Maximum motor speed
    double currentPosition = lb.get_position();
    double error = targetPosition - currentPosition;

    // If error is within tolerance, stop and exit
    if (fabs(error) <= tolerance) {
        lb.move(0);
        return;
    }

    integral += error;
    integral = fmax(fmin(integral, 100), -100);  // Anti-windup
    double derivative = error - lastError;
    double motorSpeed = (0.7 * error) + (0.0 * integral) + (0.6 * derivative);

    motorSpeed = fmax(fmin(motorSpeed, maxSpeed), -maxSpeed);
    lb.move(motorSpeed);

    lastError = error;
}

// LB control function that handles free movement and snapping
void LBControl() {
    double currentPos = lb.get_position();
    bool r1Pressed = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R1); // Down button
    bool r2Pressed = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R2); // Up button

    // --- Handle Upward Movement (R2) ---
    if (r2Pressed) {
        if (!previousR2) {  // Button just pressed
            r2PressStart = pros::millis();
        }
        // Free movement upward
        lb.move(127);
    } else if (previousR2) { // Button was released
        unsigned long duration = pros::millis() - r2PressStart;
        // If released quickly (a click), check for snapping if within preset range
        if (duration < CLICK_THRESHOLD) {
            if (fabs(currentPos - LB_PRESET_HIGH) <= SNAP_THRESHOLD ||
                fabs(currentPos - LB_PRESET_LOW)  <= SNAP_THRESHOLD) {
                // Toggle: if near high, snap to low; if near low, snap to high.
                double target;
                if (fabs(currentPos - LB_PRESET_HIGH) <= SNAP_THRESHOLD) {
                    target = LB_PRESET_LOW;
                } else {
                    target = LB_PRESET_HIGH;
                }
                // PID-controlled snap to the target preset
                while (fabs(lb.get_position() - target) > 5) {
                    NewLBMoveToTarget(target);
                    pros::delay(20);
                }
                lb.move(0);
            } else {
                // Not in preset range: simply stop
                lb.move(0);
            }
        } else {
            // If held longer, just stop free movement on release
            lb.move(0);
        }
    }

    // --- Handle Downward Movement (R1) ---
    if (r1Pressed) {
        if (!previousR1) {  // Button just pressed
            r1PressStart = pros::millis();
        }
        // Free movement downward
        lb.move(-127);
    } else if (previousR1) { // Button was released
        unsigned long duration = pros::millis() - r1PressStart;
        if (duration < CLICK_THRESHOLD) {
            if (fabs(currentPos - LB_PRESET_HIGH) <= SNAP_THRESHOLD ||
                fabs(currentPos - LB_PRESET_LOW)  <= SNAP_THRESHOLD) {
                // Toggle: if near high, snap to low; if near low, snap to high.
                double target;
                if (fabs(currentPos - LB_PRESET_HIGH) <= SNAP_THRESHOLD) {
                    target = LB_PRESET_LOW;
                } else {
                    target = LB_PRESET_HIGH;
                }
                while (fabs(lb.get_position() - target) > 5) {
                    NewLBMoveToTarget(target);
                    pros::delay(20);
                }
                lb.move(0);
            } else {
                lb.move(0);
            }
        } else {
            lb.move(0);
        }
    }

    // Update previous button states
    previousR1 = r1Pressed;
    previousR2 = r2Pressed;
}

// Called repeatedly in the control loop (or as a separate task)
void NewLBSpinToTarget() {
    new_lb_mutex.take();
    LBControl();
    new_lb_mutex.give();
}