#include "main.h"

#define LEFT_DRIVE_1_PORT = 18
#define LEFT_DRIVE_2_PORT = 20
#define LEFT_DRIVE_3_PORT = 9
#define LEFT_DRIVE_4_PORT = 10
#define RIGHT_DRIVE_1_PORT = 13
#define RIGHT_DRIVE_2_PORT = 14
#define RIGHT_DRIVE_3_PORT = 3
#define RIGHT_DRIVE_4_PORT = 4

#define LEFT_ROLLER_PORT = 19
#define RIGHT_ROLLER_PORT = 11

#define LEFT_LIFT_PORT = 16
#define RIGHT_LIFT_PORT = 5

#define TILTER_PORT = 15

Controller masterController;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);
ControllerButton trayDown(ControllerDigital::R1);
ControllerButton trayUp(ControllerDigital::R2);
ControllerButton liftUp(ControllerDigital::up);
ControllerButton liftDown(ControllerDigital::down);

Motor leftRoller(LEFT_ROLLER_PORT);
Motor rightRoller(RIGHT_ROLLER_PORT);
Motor trayMotor(TILTER_PORT);
Motor leftLift(LEFT_LIFT_PORT);
Motor rightLift(RIGHT_LIFT_PORT);


void on_center_button() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}


void initialize() {
	leftRoller.setBrakeMode(AbstractMotor::brakeMode::hold);
	rightRoller.setBrakeMode(AbstractMotor::brakeMode::hold);
	leftRoller.setGearing(AbstractMotor::gearset::green);
	rightRoller.setGearing(AbstractMotor::gearset::green);

	trayMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	trayMotor.setGearing(AbstractMotor::gearset::red);

	leftLift.setBrakeMode(AbstractMotor::brakeMode::hold);
	rightLift.setBrakeMode(AbstractMotor::brakeMode::hold);
	leftLift.setGearing(AbstractMotor::gearset::green);
	rightLift.setGearing(AbstractMotor::gearset::green);
}


/**
 * Moves both roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void rollers(int speed) {
	leftRoller.move_velocity(speed * 2);
	rightRoller.move_velocity(speed * 2);
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
 * Moves the roller lift. Speed will depend on the speed parameter. The
 * range is -100 to 100.
*/
void lift(int speed) {
	leftLift.move_velocity(speed * 2);
	rightLift.move_velocity(speed * 2);
}


/**
 * Moves the rollers to intake and outtake depending on the state of
 * the up and down buttons. If the up button is pressed, the
 * manipulator will intake, and if the down button is pressed, the
 * manipulator will outtake. The up button has priority.
*/
void rollerControl(ControllerButton inBtn, ControllerButton outBtn) {
	if (inBtn.isPressed()) {
		rollers(100);
	} else if (down.isPressed()) {
		rollers(-100);
	} else {
		rollers(0);
	}
}

/**
 * Move the tray up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority.
*/
void tilterControl(ControllerButton upBtn, ControllerButton downBtn) {
	if (downBtn.isPressed()) {
		tilter(100);
	} else if (upBtn.isPressed()) {
		tilter(-50);
	} else {
		tilter(0);
	}
}

/**
 * Move the lift up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. if
 * the down button is pressed, the tray will move downwards. Theup
 * button has priority.
*/
void liftControl(ControllerButton upBtn, ControllerButton downBtn) {
	if (upBtn.isPressed()) {
		lift(100);
	} else if (downBtn.isPressed()) {
		lift(-100);
	} else {
		lift(0);
	}
}


void opcontrol() {

	while (true) {
		rollerControl(intakeIn, intakeOut);
		tilterControl(trayUp, trayDown);
		liftControl(liftUp, liftDown);

		pros::delay(20);
	}
}
