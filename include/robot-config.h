#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include <string>

// all externed components

extern pros::Controller controller1;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern pros::Motor preroller;
extern pros::Motor intake;
extern pros::MotorGroup lb;
extern pros::Rotation lb_sensor;

extern pros::adi::DigitalOut mogo;
extern pros::adi::DigitalOut doinker;
extern pros::adi::DigitalOut doinkerPiston;

extern pros::Imu inertial;
extern pros::Optical reject;

extern pros::Rotation horizontalEnc;
extern pros::Rotation verticalEnc;
extern lemlib::TrackingWheel horizontal;
extern lemlib::TrackingWheel vertical;

extern lemlib::Drivetrain drivetrain;
extern lemlib::ControllerSettings linearController;
extern lemlib::ControllerSettings angularController;

extern lemlib::OdomSensors sensors;

extern lemlib::ExpoDriveCurve throttleCurve;
extern lemlib::ExpoDriveCurve steerCurve;

extern lemlib::Chassis chassis;

// all externed variables

extern bool previousB;
extern bool previousX;
extern bool previousY;

// all externed functions

// extern void opcontrolLoop(char allianceColour);
extern void opcontrolLoop(char allianceColour);

extern void robotInit();

extern void prerollerForward();
extern void prerollerReverse();
extern void prerollerStop();

extern pros::Mutex uptake_mutex;

extern void uptakeForward();
extern void uptakeReverse();
extern void handleUptakeState();
extern void updateUptakeMotor();

extern void mogoButton();
extern void doinkerButton();
extern void doinkerPistonButton();

extern void rejectOnOff();
extern void rejectRing();
extern void setAllianceColour(char allianceColour);

extern void turnToAngle(double targetAngleDegrees, int maxSpeed);
extern void moveDistance(double targetDistanceInches, int maxSpeed);

extern void opcontrolInit();
extern void autocontrolInit();