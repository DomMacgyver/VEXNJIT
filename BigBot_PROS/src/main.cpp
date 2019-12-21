#include "main.h"


int LEFT_ROLLER_PORT = 19;
int RIGHT_ROLLER_PORT = 11;
int LEFT_LIFT_PORT = 16;
int RIGHT_LIFT_PORT = 5;
int TILTER_PORT = 15;


Motor leftRoller(LEFT_ROLLER_PORT, false, AbstractMotor::gearset::red);
Motor rightRoller(RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::red);

Motor leftLift(LEFT_LIFT_PORT, false, AbstractMotor::gearset::green);
Motor rightLift(RIGHT_LIFT_PORT, false, AbstractMotor::gearset::green);

Motor trayMotor(TILTER_PORT, false, AbstractMotor::gearset::red);


Controller masterController;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);

ControllerButton liftUp(ControllerDigital::up);
ControllerButton liftDown(ControllerDigital::down);

ControllerButton trayDown(ControllerDigital::R1);
ControllerButton trayUp(ControllerDigital::R2);


void on_center_button() {}


void initialize() {
	leftLift.setBrakeMode(AbstractMotor::brakeMode::hold);
	rightLift.setBrakeMode(AbstractMotor::brakeMode::hold);
}


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
 * Moves the roller lift. Speed will depend on the speed parameter. The
 * range is -100 to 100.
*/
void lift(int speed) {
	leftLift.move_velocity(speed * 2);
	rightLift.move_velocity(-speed * 2);
}


/**
 * Moves the tray motor. Speed will depend on the speed parameter. The
 * range is -100 to 100. If speed is 0, the motor will stop with a
 * brakeType of "hold."
*/
void tilter(int speed) {
	trayMotor.move_velocity(speed);
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


/**
 * Move the lift up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority.
*/
void liftControl() {
	if (liftUp.isPressed()) {
		lift(100);
	} else if (liftDown.isPressed()) {
		lift(-100);
	} else {
		lift(0);
	}
}


/**
 * Move the tray up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority.
*/
void tilterControl() {
	if (trayUp.isPressed()) {
		tilter(100);
	} else if (trayDown.isPressed()) {
		tilter(-100);
	} else {
		tilter(0);
	}
}


void opcontrol() {
	while(true) {
		rollersControl();
		liftControl();
		tilterControl();

		pros::delay(20);
	}
}
