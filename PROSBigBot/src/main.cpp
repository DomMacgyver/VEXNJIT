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

Controller masterController;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);

Motor leftRoller(LEFT_ROLLER_PORT);
Motor rightRoller(RIGHT_ROLLER_PORT);


void on_center_button() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}


void initialize() {
	leftRoller.setBrakeMode(AbstractMotor::brakeMode::hold);
	rightRoller.setBrakeMode(AbstractMotor::brakeMode::hold);
	leftRoller.setGearing(AbstractMotor::gearset::green);
	rightRoller.setGearing(AbstractMotor::gearset::green);
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

		rollerControl(intakeIn, intakeOut);

		pros::delay(20);
	}
}
