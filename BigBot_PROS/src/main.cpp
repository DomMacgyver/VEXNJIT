#include "main.h"


int LEFT_ROLLER_PORT = 19;
int RIGHT_ROLLER_PORT = 11;

Controller masterController;

Motor leftRoller(LEFT_ROLLER_PORT, false, AbstractMotor::gearset::red);
Motor rightRoller(RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::red);

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);

void on_center_button() {}
void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

/**
 * Moves both roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void rollers(int speed) {
	leftRoller.move_velocity(-speed);
	rightRoller.move_velocity(speed);
}


/**
 * Moves the rollers to intake and outtake depending on the state of
 * the up and down buttons. If the up button is pressed, the
 * manipulator will intake, and if the down button is pressed, the
 * manipulator will outtake. The up button has priority.
*/
void rollersControl() {
	if (intakeIn.isPressed()) {
		rollers(100);
	} else if (intakeOut.isPressed()) {
		rollers(-100);
	} else {
		rollers(0);
	}
}


void opcontrol() {

	while(true) {
		rollersControl();

		pros::delay(20);
	}

}
