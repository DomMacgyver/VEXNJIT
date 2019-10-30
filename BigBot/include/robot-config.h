using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor LeftIntake;
extern motor RightIntake;
extern controller Controller1;
extern motor LeftDrive1;
extern motor LeftDrive2;
extern motor LeftDrive3;
extern motor LeftDrive4;
extern motor RightDrive1;
extern motor RightDrive2;
extern motor RightDrive3;
extern motor RightDrive4;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );