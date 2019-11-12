/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       dalenash                                                  */
/*    Created:      Wed Oct 30 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftIntake           motor         19             
// RightIntake          motor         11              
// Controller1          controller                    
// LeftDrive1           motor         18              
// LeftDrive2           motor         20              
// LeftDrive3           motor         9               
// LeftDrive4           motor         10              
// RightDrive1          motor         13              
// RightDrive2          motor         14              
// RightDrive3          motor         3               
// RightDrive4          motor         4               
// TrayMotor            motor         15              
// trayPot              pot           A               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

/**
  * Moves both roller motors. Speed will depend on the speed
  * parameter. The range is -100 to 100.
*/
void rollers(int speed) {
    LeftIntake.spin(directionType::fwd, speed, velocityUnits::pct);
    RightIntake.spin(directionType::rev, speed, velocityUnits::pct);
}

/** 
 * Moves the rollers to intake and outtake depending on the state of
 * the up and down buttons. If the up button is pressed, the manipulator
 * will intake, and if the down button is pressed, the manipulator will
 * outtake. The up button has priority.
*/
void manipulator(controller::button inBtn, controller::button outBtn) {
    if (inBtn.pressing()) {
        rollers(-100);
    } else if (outBtn.pressing()) {
        rollers(100);
    } else {
        rollers(0);
    }
}

/**
  * Moves the tray motor. Speed will depend on the speed parameter.
  * The range is -100 to 100. If speed is 0, the motor will stop with
  * a brakeType of "hold." 
*/
void tray(int speed) {
    if (speed == 0) {
        TrayMotor.stop(brakeType::hold);
    } else {
        TrayMotor.spin(directionType::fwd, speed, velocityUnits::pct);
    }
}

/** 
 * Move the tray up or down, depending on the state of the up and
 * down buttons. If the up button is pressed, the tray will move upwards.
 * If the down button is pressed, the tray will move downwards. The up
 * button has priority.
*/
void trayLift(controller::button inBtn, controller::button outBtn) {
    if (inBtn.pressing()) {
        tray(100);
    } else if (outBtn.pressing()) {
        tray(-100);
    } else {
        tray(0);
    }
}

/** 
 * Moves the drivetrain based on the "arcade" controls.
 * There is no priority, the two values are just added
 * together. For example, if input movements to move mostly
 * forward and slightly to the right, the left side of the
 * drive will move slightly faster than the right side.
 */
void drive(controller::axis axis3, controller::axis axis1) {
    int val1 = axis1.value();
    int val3 = axis3.value();

    RightDrive1.spin(directionType::fwd, val1 - val3, percentUnits::pct);
    RightDrive2.spin(directionType::fwd, val3 - val1, percentUnits::pct);
    RightDrive3.spin(directionType::fwd, val3 - val1, percentUnits::pct);
    RightDrive4.spin(directionType::fwd, val1 - val3, percentUnits::pct);
    LeftDrive1.spin(directionType::fwd, val3 + val1, percentUnits::pct);
    LeftDrive2.spin(directionType::fwd, -val1 - val3, percentUnits::pct);
    LeftDrive3.spin(directionType::fwd, -val1 - val3, percentUnits::pct);
    LeftDrive4.spin(directionType::fwd, val3 + val1, percentUnits::pct);
} 


void usercontrol(void) {
    while(1) {
        controller::button INTAKE_IN = Controller1.ButtonR1;
        controller::button INTAKE_OUT = Controller1.ButtonR2;

        controller::button TRAY_UP = Controller1.ButtonL1;
        controller::button TRAY_DOWN = Controller1.ButtonL2;

        controller::axis AXIS3 = Controller1.Axis3;
        controller::axis AXIS1 = Controller1.Axis1;

        drive(AXIS3, AXIS1);
        
        manipulator(INTAKE_IN, INTAKE_OUT);

        trayLift(TRAY_UP, TRAY_DOWN);

        wait(20, msec);
    }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
    vexcodeInit();

    Competition.drivercontrol(usercontrol);

    while(1) {
        wait(100, msec);
    }

}
