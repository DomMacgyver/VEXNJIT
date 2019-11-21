#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor LeftIntake = motor(PORT19, ratio18_1, false);
motor RightIntake = motor(PORT11, ratio18_1, false);
controller Controller1 = controller(primary);
motor LeftDrive1 = motor(PORT18, ratio18_1, false);
motor LeftDrive2 = motor(PORT20, ratio18_1, false);
motor LeftDrive3 = motor(PORT9, ratio18_1, false);
motor LeftDrive4 = motor(PORT10, ratio18_1, false);
motor RightDrive1 = motor(PORT13, ratio18_1, false);
motor RightDrive2 = motor(PORT14, ratio18_1, false);
motor RightDrive3 = motor(PORT3, ratio18_1, false);
motor RightDrive4 = motor(PORT4, ratio18_1, false);
motor LeftRollerLift = motor(PORT5, ratio18_1, false);
motor TrayMotor = motor(PORT15, ratio36_1, false);
pot TrayPot = pot(Brain.ThreeWirePort.A);
motor RightRollerLift = motor(PORT12, ratio18_1, false);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}