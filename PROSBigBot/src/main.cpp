#include "main.h"

#define LEFT_DRIVE_1_PORT = 18
#define LEFT_DRIVE_2_PORT = 20
#define LEFT_DRIVE_3_PORT = 9
#define LEFT_DRIVE_4_PORT = 10
#define RIGHT_DRIVE_1_PORT = 13
#define RIGHT_DRIVE_2_PORT = 14
#define RIGHT_DRIVE_3_PORT = 3
#define RIGHT_DRIVE_4_PORT = 4

// #define LEFT_INTAKE_PORT = 19
// #define RIGHT_INTAKE_PORT = 11

// #define LEFT_LIFT_PORT = 16
// #define RIGHT_LIFT_PORT = 5

Controller masterController;


void on_center_button() {}
void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}


auto tankChassis = ChassisControllerFactory::create(
	{LEFT_DRIVE_1_PORT, -LEFT_DRIVE_2_PORT, -LEFT_DRIVE_3_PORT, LEFT_DRIVE_4_PORT},
	{-RIGHT_DRIVE_1_PORT, RIGHT_DRIVE_2_PORT, RIGHT_DRIVE_3_PORT, -RIGHT_DRIVE_4_PORT},
	Abstract::gearset::green,
	{4.00_in, 16.00_in}
);

void opcontrol() {

	while (true) {
		tankChassis.arcade(
			masterController.getAnalog(ControllerAnalog::leftY),
			masterController.getAnalog(ControllerAnalog::rightX)
		);

		pros::delay(20);
	}
}
