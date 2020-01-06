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

Motor leftClampMotor(LEFT_CLAMP_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor rightClampMotor(RIGHT_CLAMP_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor topLeftLiftMotor(TOP_LEFT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor bottomLeftLiftMotor(BOTTOM_LEFT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor topRightLiftMotor(TOP_RIGHT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor bottomRightLiftMotor(BOTTOM_RIGHT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor leftFrontDriveMotor(LEFT_FRONT_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor leftBackDriveMotor(LEFT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rightFrontDriveMotor(RIGHT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rightBackDriveMotor(RIGHT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);


ControllerButton clampInBtn(ControllerDigital::L1);
ControllerButton clampOutBtn(ControllerDigital::L2);
ControllerButton liftUpBtn(ControllerDigital::R1);
ControllerButton liftDownBtn(ControllerDigital::R2);

ControllerButton presetX(ControllerDigital::X);
ControllerButton presetA(ControllerDigital::A);
ControllerButton presetB(ControllerDigital::B);


auto chassis = ChassisControllerBuilder()
	.withMotors(
		leftFrontDriveMotor,
		rightFrontDriveMotor,
		rightBackDriveMotor,
		leftBackDriveMotor
	).withDimensions(
		AbstractMotor::gearset::green,
		{
			{4.0_in, 11.0_in},
			imev5GreenTPR
		}
	).withOdometry().buildOdometry();

auto model = static_pointer_cast<XDriveModel>(chassis->getModel());


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
}



void disabled() {}
void competition_initialize() {}


void drive(double forward, double strafe, double turn) {
	model->xArcade(
		strafe,
		forward,
		turn
	);
}


/**
 * Moves both roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void clamp(int speed) {
	leftClampMotor.moveVelocity(speed);
	rightClampMotor.moveVelocity(-speed);
}


/**
 * Moves the clamp to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void clampPosition(int pos, int speed) {
	leftClampMotor.moveAbsolute(pos, speed);
	rightClampMotor.moveAbsolute(-pos, speed);
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
	topLeftLiftMotor.moveVelocity(-speed);
	topRightLiftMotor.moveVelocity(speed);
	bottomLeftLiftMotor.moveVelocity(speed);
	bottomRightLiftMotor.moveVelocity(-speed);
}


/**
 * Moves the lift to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void liftPosition(int pos, int speed) {
	topLeftLiftMotor.moveAbsolute(-pos, speed);
	bottomLeftLiftMotor.moveAbsolute(pos, speed);
	topRightLiftMotor.moveAbsolute(pos, speed);
	bottomRightLiftMotor.moveAbsolute(-pos, speed);
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
	} else if (liftDownBtn.isPressed() && topLeftLiftMotor.getPosition() < -30) {
		lift(-50);
	}

	if (liftUpBtn.changedToReleased() || liftDownBtn.changedToReleased()) {
		lift(0);
	}
}


void driveControl() {
	double forward = joystick.getAnalog(ControllerAnalog::leftY) * 0.9;
	double strafe = joystick.getAnalog(ControllerAnalog::leftX);
	double turn = joystick.getAnalog(ControllerAnalog::rightX) * 0.7;

	if (fabs(strafe) < .15) {
		strafe = 0.0;
	}
	drive(forward, strafe, turn);
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





void forward(QLength x, QLength y, int speed) {
	chassis->setMaxVelocity(speed * 2);
	chassis->driveToPoint({x, y});
	chassis->setMaxVelocity(200);
}

void backward(QLength x, QLength y, int speed) {
	chassis->setMaxVelocity(speed * 2);
	chassis->driveToPoint({x, y}, true);
	chassis->setMaxVelocity(200);
}

void turn(QAngle angle, int speed) {
	chassis->setMaxVelocity(speed * 2);
	chassis->turnToAngle(angle);
	chassis->setMaxVelocity(200);
}


void autonomous() {
	model->setMaxVelocity(70);
	model->forward(-70);
	pros::delay(1100);
	model->stop();
	pros::delay(1000);
	model->forward(40);
	pros::delay(500);
	model->stop();
	model->setMaxVelocity(200);
}



void opcontrol() {
	while (true) {
		clampControl();
		liftControl();
		driveControl();
		liftPresets();

		pros::delay(20);
	}
}
