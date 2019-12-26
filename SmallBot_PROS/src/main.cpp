#include "main.h"

using namespace std;

int LEFT_FRONT_DRIVE_PORT = 20;
int LEFT_BACK_DRIVE_PORT = 10;
int RIGHT_FRONT_DRIVE_PORT = 11;
int RIGHT_BACK_DRIVE_PORT = 1;
int LEFT_CLAMP_PORT = 19;
int RIGHT_CLAMP_PORT = 12;

Controller joystick;

Motor leftClampMotor(LEFT_CLAMP_PORT, false, AbstractMotor::gearset::red);
Motor rightClampMotor(RIGHT_CLAMP_PORT, false, AbstractMotor::gearset::red);

ControllerButton clampBtn(ControllerDigital::L1);


auto chassis = ChassisModelFactory::create(
	LEFT_FRONT_DRIVE_PORT,
	RIGHT_FRONT_DRIVE_PORT,
	RIGHT_BACK_DRIVE_PORT,
	LEFT_BACK_DRIVE_PORT,
	200.0
);



void on_center_button() {}



void initialize() {
	leftClampMotor.tarePosition();
	rightClampMotor.tarePosition();
}



void disabled() {}
void competition_initialize() {}
void autonomous() {}


/**
 * Moves the clamp to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void clampPosition(int pos, int speed) {
	leftClampMotor.move_absolute(pos, speed);
	rightClampMotor.move_absolute(-pos, speed);
}


/**
 * Moves the clamp in our out, depending on when the L1 button is
 * pressed. If the clamp is already inwards, pressing it will move it
 * out. If it is already outwards, pressing it will move it in.
*/
void clampControl() {
	if (clampBtn.changedToPressed()) {
		if (leftClampMotor.get_position() < 20) {
			clampPosition(300, 10);
		} else if (leftClampMotor.get_position() > 280) {
			clampPosition(0, 10);
		}
	}
}


void opcontrol() {
	while (true) {
		chassis.xArcade(
			joystick.getAnalog(ControllerAnalog::leftX) * 0.9,
			joystick.getAnalog(ControllerAnalog::leftY),
			joystick.getAnalog(ControllerAnalog::rightX) * 0.7
		);

		clampControl();

		pros::delay(20);
	}
}
