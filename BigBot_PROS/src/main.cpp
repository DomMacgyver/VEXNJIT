#include "main.h"

using namespace std;

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

ControllerButton presetX(ControllerDigital::X);
ControllerButton presetB(ControllerDigital::B);

ControllerButton slowBackBtn(ControllerDigital::left);


auto chassis = ChassisControllerFactory::create(
	{-LEFT_DRIVE_1_PORT, LEFT_DRIVE_2_PORT, LEFT_DRIVE_3_PORT, -LEFT_DRIVE_4_PORT},
	{RIGHT_DRIVE_1_PORT, -RIGHT_DRIVE_2_PORT, -RIGHT_DRIVE_3_PORT, RIGHT_DRIVE_4_PORT},
	AbstractMotor::gearset::green,
	{4_in, 16_in}
);
auto motion = AsyncControllerFactory::motionProfile(
	0.25,
	0.3,
	1.6,
	chassis
);
auto drive = ChassisModelFactory::create(
	{LEFT_DRIVE_1_PORT, -LEFT_DRIVE_2_PORT, -LEFT_DRIVE_3_PORT, LEFT_DRIVE_4_PORT},
	{-RIGHT_DRIVE_1_PORT, RIGHT_DRIVE_2_PORT, RIGHT_DRIVE_3_PORT, -RIGHT_DRIVE_4_PORT},
	200.0
);


void on_center_button() {}


void initialize() {
	leftLift.setBrakeMode(AbstractMotor::brakeMode::hold);
	rightLift.setBrakeMode(AbstractMotor::brakeMode::hold);
	trayMotor.setBrakeMode(AbstractMotor::brakeMode::hold);

	leftLift.tarePosition();
	rightLift.tarePosition();

	motion.generatePath(
		{
			Point{0_ft, 0_ft, 0_deg},
			Point{2_ft, 2_ft, 45_deg}
		},
		"A"
	);

	pros::lcd::initialize();
}


void disabled() {}
void competition_initialize() {}


void autonomous() {
	motion.setTarget("A", true);
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
 * Moves the roller lift to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void liftPosition(int pos, int speed) {
	leftLift.move_absolute(pos, speed);
	rightLift.move_absolute(-pos, speed);
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
		lift(90);
	} else if (liftDown.isPressed()) {
		lift(-90);
	}

	if (liftUp.changedToReleased() || liftDown.changedToReleased()) {
		lift(0);
	}
}


/**
 * Moves the lift to a specific height, depending on the button pressed
 * in the XYAB button mapping. If there is a significant difference
 * between the two arms, they will stop and automatically re-align.
*/
void liftPresets() {
	if (presetX.isPressed()) {
		liftPosition(1100, 110);
	}
	if (presetB.isPressed()) {
		liftPosition(0, 110);
	}

	int diff = abs(leftLift.get_position()) - abs(rightLift.get_position());

	if (abs(diff) > 90) {
		int direction = leftLift.get_direction();

		int reset_speed = 80;
		if (direction == 1) {
			if (diff > 0) {
				rightLift.move_velocity(0);
				leftLift.move_absolute(-rightLift.get_position(), reset_speed);
			} else {
				leftLift.move_velocity(0);
				rightLift.move_absolute(-leftLift.get_position(), reset_speed);
			}
		} else {
			if (diff > 0) {
				rightLift.move_velocity(0);
				leftLift.move_absolute(-rightLift.get_position(), reset_speed);
			} else {
				leftLift.move_velocity(0);
				rightLift.move_absolute(-leftLift.get_position(), reset_speed);
			}
		}
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


/**
 * Slowly moves the drive backwards while moving the rollers forward.
 * Use this to safely back out after scoring.
*/
void slowBack() {
	if (slowBackBtn.isPressed()) {
		drive.forward(-.07);
		rollers(25);
	}
}


void opcontrol() {
	while(true) {
		drive.arcade(
			joystick.getAnalog(ControllerAnalog::leftY),
			joystick.getAnalog(ControllerAnalog::rightX)
		);

		rollersControl();
		liftControl();
		tilterControl();
		liftPresets();
		slowBack();

		pros::delay(20);
	}
}
