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

#include <iostream>

void opcontrolLoop(void* param) {
    leftMotors.set_current_limit(2500);
    rightMotors.set_current_limit(2500);
    uptake.set_current_limit(2500);
    preroller.set_current_limit(2500);

    leftMotors.set_reversed(true);
    rightMotors.set_reversed(false);

    // leftMotors.move_velocity(200);
    // rightMotors.move_velocity(200);

    preroller.move_velocity(200);
    uptake.move_velocity(200);
    
    preroller.set_brake_mode (pros::E_MOTOR_BRAKE_COAST);
    uptake.set_brake_mode (pros::E_MOTOR_BRAKE_COAST);
    lb.set_brake_mode (pros::E_MOTOR_BRAKE_HOLD);

    reject.set_led_pwm(100);
    
    lb.tare_position();

    // andy likes SPLIT ARCADE drive
    // tank drive is too confusing :(

    // this runs during tele-op the whole time
    while (true) {
        // get left y and right x positions
        int leftY = controller1.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller1.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        // please, robot, move.
        chassis.arcade(leftY, rightX);

        if (controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            prerollerForward();
        } else if (controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            prerollerReverse();
        } else {
            prerollerStop();
        }

        // if (controller1.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        //     LBForward();
        // } else if (controller1.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        //     LBReverse();
        // } else {
        //     LBStop();
        // }

        if (controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            uptakeForward(); // Forward has priority
        } else if (controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            uptakeReverse(); // Reverse handled second
        }

        handleUptakeState();
        updateUptakeMotor();

        // Handle B button with debounce
        bool currentB = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_B);
        if (currentB && !previousB) {
            mogoButton();
        }
        previousB = currentB;


        // Handle X button with debounce
        bool currentX = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_X);
        if (currentX && !previousX) {
            rejectOnOff();
        }
        previousX = currentX;

        // Handle Y button with debounce
        bool currentY = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_Y);
        if (currentY && !previousY) {
            doinkerButton();
        }
        previousY = currentY;

        rejectRing();

        // lb.move_absolute(-500, 50);

        lb.move_absolute(-210, 20);

        // delay so the Brain doesn't explode
        pros::delay(25);
    }
}

// Initialize robot and start the task
void robotInit() {
    pros::Task opcontrolLoopTask(opcontrolLoop);
}