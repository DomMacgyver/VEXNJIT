#include "main.h"


int LEFT_ROLLER_PORT = 19;
int RIGHT_ROLLER_PORT = 11;
int LEFT_LIFT_PORT = 16;
int RIGHT_LIFT_PORT = 5;
int TILTER_PORT = 15;
int LEFT_DRIVE_1_PORT = 18;
int LEFT_DRIVE_2_PORT = 20;
int LEFT_DRIVE_3_PORT = 9;
int LEFT_DRIVE_4_PORT = 10;
int RIGHT_DRIVE_1_PORT = 13;
int RIGHT_DRIVE_2_PORT = 14;
int RIGHT_DRIVE_3_PORT = 3;
int RIGHT_DRIVE_4_PORT = 4;

Motor leftRoller(LEFT_ROLLER_PORT, false, AbstractMotor::gearset::red);
Motor rightRoller(RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::red);

Motor leftLift(LEFT_LIFT_PORT, false, AbstractMotor::gearset::green);
Motor rightLift(RIGHT_LIFT_PORT, false, AbstractMotor::gearset::green);

Motor trayMotor(TILTER_PORT, false, AbstractMotor::gearset::red);


Controller joystick;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);

ControllerButton liftUp(ControllerDigital::up);
ControllerButton liftDown(ControllerDigital::down);

ControllerButton trayDown(ControllerDigital::R1);
ControllerButton trayUp(ControllerDigital::R2);


auto chassis = ChassisControllerFactory::create(
	{-LEFT_DRIVE_1_PORT, LEFT_DRIVE_2_PORT, LEFT_DRIVE_3_PORT, -LEFT_DRIVE_4_PORT},
	{RIGHT_DRIVE_1_PORT, -RIGHT_DRIVE_2_PORT, -RIGHT_DRIVE_3_PORT, RIGHT_DRIVE_4_PORT},
	AbstractMotor::gearset::green,
	{4_in, 16_in}
);
auto motion = AsyncControllerFactory::motionProfile(
	0.3,
	0.8,
	5.0,
	chassis
);


void on_center_button() {}


void initialize() {
	leftLift.setBrakeMode(AbstractMotor::brakeMode::hold);
	rightLift.setBrakeMode(AbstractMotor::brakeMode::hold);
	trayMotor.setBrakeMode(AbstractMotor::brakeMode::hold);

	motion.generatePath(
		{
			Point{0_ft, 0_ft, 0_deg},
			Point{2_ft, 1_ft, 0_deg}
		},
		"A"
	);

}


void disabled() {}
void competition_initialize() {}


void autonomous() {
	motion.setTarget("A");
	motion.waitUntilSettled();
}


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

auto drive = ChassisModelFactory::create(
	{LEFT_DRIVE_1_PORT, -LEFT_DRIVE_2_PORT, -LEFT_DRIVE_3_PORT, LEFT_DRIVE_4_PORT},
	{-RIGHT_DRIVE_1_PORT, RIGHT_DRIVE_2_PORT, RIGHT_DRIVE_3_PORT, -RIGHT_DRIVE_4_PORT},
	200.0
);

void opcontrol() {
	while(true) {
		drive.arcade(
			joystick.getAnalog(ControllerAnalog::leftY),
			joystick.getAnalog(ControllerAnalog::rightX)
		);

		rollersControl();
		liftControl();
		tilterControl();

		pros::delay(20);
	}
}
