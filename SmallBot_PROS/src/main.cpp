#include "main.h"

using namespace std;

int LEFT_FRONT_DRIVE_PORT = 20;
int LEFT_BACK_DRIVE_PORT = 10;
int RIGHT_FRONT_DRIVE_PORT = 11;
int RIGHT_BACK_DRIVE_PORT = 1;

Controller joystick;

auto chassis = ChassisModelFactory::create(
	LEFT_FRONT_DRIVE_PORT,
	RIGHT_FRONT_DRIVE_PORT,
	RIGHT_BACK_DRIVE_PORT,
	LEFT_BACK_DRIVE_PORT,
	200.0
);


void on_center_button() {}
void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}



void opcontrol() {
	while (true) {
		chassis.xArcade(
			0,
			joystick.getAnalog(ControllerAnalog::leftY),
			joystick.getAnalog(ControllerAnalog::rightX) * 0.7
		);

		pros::delay(20);
	}
}
