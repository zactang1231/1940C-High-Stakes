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

enum lbState {
    LOADING,
    DEFAULT,
    SCORING
};

const double LB_PRESET_HIGH = -840;   
const double LB_PRESET_LOW  = -290;  
const double SNAP_THRESHOLD = 10;   
const int    CLICK_THRESHOLD = 500; 

extern lbState LBState;
extern double LBTargetPos;
extern unsigned long r1PressStart;  // Time when R1 (down) was pressed
extern unsigned long r2PressStart;  // Time when R2 (up) was pressed
extern bool previousR1;             // Previous state of R1 button
extern bool previousR2;             // Previous state of R2 button

extern void NewLBSpinToTarget();
