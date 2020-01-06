#include "main.h"

using namespace std;

int LEFT_FRONT_DRIVE_PORT = 20;
int LEFT_BACK_DRIVE_PORT = 10;
int RIGHT_FRONT_DRIVE_PORT = 11;
int RIGHT_BACK_DRIVE_PORT = 1;
int LEFT_CLAMP_PORT = 19;
int RIGHT_CLAMP_PORT = 12;
int TOP_LEFT_LIFT_PORT = 9;
int BOTTOM_LEFT_LIFT_PORT = 8;
int TOP_RIGHT_LIFT_PORT = 2;
int BOTTOM_RIGHT_LIFT_PORT = 3;

Controller joystick;

Motor leftClampMotor(LEFT_CLAMP_PORT, false, AbstractMotor::gearset::red);
Motor rightClampMotor(RIGHT_CLAMP_PORT, false, AbstractMotor::gearset::red);
Motor topLeftLiftMotor(TOP_LEFT_LIFT_PORT, false, AbstractMotor::gearset::red);
Motor bottomLeftLiftMotor(BOTTOM_LEFT_LIFT_PORT, false, AbstractMotor::gearset::red);
Motor topRightLiftMotor(TOP_RIGHT_LIFT_PORT, false, AbstractMotor::gearset::red);
Motor bottomRightLiftMotor(BOTTOM_RIGHT_LIFT_PORT, false, AbstractMotor::gearset::red);
Motor leftFrontDriveMotor(LEFT_FRONT_DRIVE_PORT, false, AbstractMotor::gearset::green);
Motor leftBackDriveMotor(LEFT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green);
Motor rightFrontDriveMotor(RIGHT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green);
Motor rightBackDriveMotor(RIGHT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green);


ControllerButton clampInBtn(ControllerDigital::L1);
ControllerButton clampOutBtn(ControllerDigital::L2);
ControllerButton liftUpBtn(ControllerDigital::R1);
ControllerButton liftDownBtn(ControllerDigital::R2);

ControllerButton presetX(ControllerDigital::X);
ControllerButton presetA(ControllerDigital::A);
ControllerButton presetB(ControllerDigital::B);


auto chassis = ChassisModelFactory::create(
	leftFrontDriveMotor,
	rightFrontDriveMotor,
	rightBackDriveMotor,
	leftBackDriveMotor,
	200.0
);

auto autonChassis = ChassisControllerFactory::create(
	leftFrontDriveMotor,
	rightFrontDriveMotor,
	rightBackDriveMotor,
	leftBackDriveMotor,
	AbstractMotor::gearset::green,
	{4.0_in, 11.0_in}
);

auto motion = AsyncControllerFactory::motionProfile(
	0.22,
	0.25,
	1.2,
	autonChassis
);



void on_center_button() {}



void initialize() {
	leftClampMotor.tarePosition();
	rightClampMotor.tarePosition();
	topLeftLiftMotor.tarePosition();

	topLeftLiftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	topRightLiftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	bottomLeftLiftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	bottomRightLiftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);

	leftClampMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	rightClampMotor.setBrakeMode(AbstractMotor::brakeMode::hold);

	pros::lcd::initialize();

	motion.generatePath(
		{
			Point{0_ft, 0_ft, 0_deg},
			Point{2_ft, 0_ft, 0_deg}
		},
		"A"
	);
}



void disabled() {}
void competition_initialize() {}


/**
 * Moves both roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void clamp(int speed) {
	leftClampMotor.move_velocity(speed);
	rightClampMotor.move_velocity(-speed);
}


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
	if (clampInBtn.isPressed()) {
		clamp(60);
	} else if (clampOutBtn.isPressed()) {
		clamp(-60);
	} else {
		clamp(0);
	}
}


/**
 * Moves the lift. Speed will depend on the speed parameter. The
 * range is -100 to 100.
*/
void lift(int speed) {
	topLeftLiftMotor.move_velocity(-speed);
	topRightLiftMotor.move_velocity(speed);
	bottomLeftLiftMotor.move_velocity(speed);
	bottomRightLiftMotor.move_velocity(-speed);
}


/**
 * Moves the lift to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void liftPosition(int pos, int speed) {
	topLeftLiftMotor.move_absolute(-pos, speed);
	bottomLeftLiftMotor.move_absolute(pos, speed);
	topRightLiftMotor.move_absolute(pos, speed);
	bottomRightLiftMotor.move_absolute(-pos, speed);
}


/**
 * Move the lift up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority. The lift will not go down if it is below
 * a point that is slightly lower than the lift's starting position.
*/
void liftControl() {
	if (liftUpBtn.isPressed()) {
		lift(80);
	} else if (liftDownBtn.isPressed() && topLeftLiftMotor.get_position() < -30) {
		lift(-50);
	}

	if (liftUpBtn.changedToReleased() || liftDownBtn.changedToReleased()) {
		lift(0);
	}
}


/**
 * Moves the lift to a specific height, depending on the button pressed
 * in the XYAB button mapping.
*/
void liftPresets() {
	if (presetX.isPressed()) {
		liftPosition(330, 50);
	}
	if (presetA.isPressed()) {
		liftPosition(240, 50);
	}
	if (presetB.isPressed()) {
		liftPosition(0, 50);
	}
}


void move(QLength distance, int speed) {
	autonChassis.setMaxVelocity(speed * 2);
	autonChassis.moveDistance(distance);

	autonChassis.setMaxVelocity(200);
}


void autonomous() {
	motion.setTarget("A", false);
	motion.waitUntilSettled();
}



void opcontrol() {
	while (true) {
		chassis.xArcade(
			joystick.getAnalog(ControllerAnalog::leftX) * 0.9,
			joystick.getAnalog(ControllerAnalog::leftY),
			joystick.getAnalog(ControllerAnalog::rightX) * 0.7
		);

		clampControl();
		liftControl();
		liftPresets();


		pros::delay(20);
	}
}
