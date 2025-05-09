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

void bluePos() {
  // same brake modes
  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  lb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  curState = 0;
  // chassis.setPose(-51, -60, 90);
  chassis.setPose(62,-33,90);
  chassis.moveToPose(38.772,-32.984,90,1500, {.forwards = false});
  chassis.turnToPoint(24, -23.5, 1500, {.forwards = false});
  chassis.moveToPoint(24,-23.5,2000, {.forwards = false, .maxSpeed = 80});
  pros::delay(800);
  mogo.set_value(true);
  pros::delay(1500);
  intake.move(-127);
  chassis.turnToPoint(23.5, -47, 1000);
  chassis.moveToPoint(23.5,-50,1000);
  chassis.turnToHeading(0,1000);
  // pros::delay(500);
  // chassis.moveToPoint(50,-50,4000, {.forwards = false});
  // pros::delay(500);
  // chassis.moveToPoint(59,-59,4000);

  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);


  // curState = 2;
  // // flip X (–51 → +51) and flip your initial heading (  90 → –90)
  // chassis.setPose(+51, -60, -90);

  // // 1) drive out to the first point, same timeout & speed
  // chassis.moveToPoint(+13, -60, 4000, {.maxSpeed = 80});
  // pros::delay(1500);

  // // 2) lift up
  // upState();
  // upState();
  // // upState();
  // // spros::delay(400);

  // // 3) turn the mirror of  48° → –48°
  // chassis.turnToHeading(-38, 1000);

  // // 4) drive back in X, but backwards; (-16 → +16)
  // chassis.moveToPoint(+16, -63, 4000, {.forwards = false});

  // // 5) drop all the way
  // downState();
  // downState();
  // downState();
  // downState();

  // chassis.moveToPoint(+13, -60, 4000, {.forwards = false});

  // // pros::delay(1000000);
  // // leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  // // rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  // chassis.moveToPoint(17, -66, 4000, {.forwards = false});

  // // 6) deploy doinker
  // doinkerPiston.set_value(true);
  // pros::delay(500);

  // // 7) turn –32° instead of +32°
  // chassis.turnToHeading(-65, 700);
  // pros::delay(300);

  // // 8) swing in for the cone (-10 → +10), still backwards
  // chassis.moveToPoint(+10, -64, 4000, {.forwards = false});

  // pros::delay(1000);

  // // 9) drive all the way back to start pose (51,–60,–90), backwards
  // chassis.moveToPose(+51, -60, -90, 4000, {.forwards = false, .maxSpeed = 80});
  // pros::delay(2800);

  // // 10) retract doinker
  // doinkerPiston.set_value(false);

  // // 11) three quick mirror‐turns
  // chassis.turnToHeading(-60, 300);
  // chassis.turnToHeading(-120, 300);
  // chassis.turnToHeading(-90, 300);

  // // 12) start intake, turn to point (+23.5,–50)
  // intake.move(-127);
  // chassis.turnToPoint(+23.5, -50, 800);
  // chassis.moveToPoint(   +23.5, -50, 2000);

  // // 13) swing in for the cube (+23.5,–23.5) backwards
  // chassis.turnToPoint(+23.5, -23.5, 800, {.forwards = false});
  // intake.move(0);

  // // 14) back away to (+23.5,–14)
  // chassis.moveToPoint(+18, -14, 5000, {.forwards = false, .maxSpeed = 40});
  // pros::delay(1800);

  // // 15) dump mobile goal & run intake
  // mogo.set_value(true);
  // intake.move(-127);
}