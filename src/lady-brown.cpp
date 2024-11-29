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
lbState LBState = DEFAULT;

bool previousRight = false;
bool previousDown = false;

float LBTargetPos = 210;

pros::Mutex lb_mutex;

void handleLBStateDown() {
    if (LBState == SCORING) {
        LBState = DEFAULT; // Go to default if scoring 
        controller1.print (0,0,"scoring-default");
    } else if (LBState == DEFAULT) {
        LBState = LOADING; // Go to loading if default
        controller1.print (0,0,"default-loading");
    } else if (LBState == LOADING) {
        // Stay in LOADING if already there
        controller1.print (0,0,"loading");
    }
}

void handleLBStateUp() {
    if (LBState == LOADING) {
        LBState = DEFAULT; // Go to default if loading 
        controller1.print (0,0,"loading-default");
    } else if (LBState == DEFAULT) {
        LBState = SCORING; // Go to scoring if default
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
            LBTargetPos = 0;
            std::cout << "Lady Brown: Scoring" << std::endl;
            break;
        case DEFAULT:
            // lb.move_absolute(-210, 100);
            LBTargetPos = -210;
            std::cout << "Lady Brown: Default" << std::endl;
            break;
        case SCORING:
            // lb.move_absolute(-500, 100);
            LBTargetPos = -720;
            std::cout << "Lady Brown: Loading" << std::endl;
            break;
    }

    lb_mutex.give(); // release mutex (uptake can be used elsewhere)
}

void LBSpinToTarget() {
    while (true) {
        bool currentRight = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT);
        bool currentDown = controller1.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

        if (currentRight && !previousRight) {
            handleLBStateUp();
        }
        if (currentDown && !previousDown) {
            handleLBStateDown();
        }

        previousRight = currentRight;
        previousDown = currentDown; 

    updateLBMotor();
        updateLBMotor();
        lb.move_absolute(LBTargetPos, 70);

        // delay so the Brain doesn't explode
        pros::delay(25);
    }
}