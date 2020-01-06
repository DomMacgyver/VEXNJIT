#include "main.h"

using namespace std;

int8_t LEFT_ROLLER_PORT = 19;
int8_t RIGHT_ROLLER_PORT = 11;
int8_t LEFT_LIFT_PORT = 16;
int8_t RIGHT_LIFT_PORT = 5;
int8_t TILTER_PORT = 15;
int8_t LEFT_DRIVE_1_PORT = 18;
int8_t LEFT_DRIVE_2_PORT = 20;
int8_t LEFT_DRIVE_3_PORT = 9;
int8_t LEFT_DRIVE_4_PORT = 10;
int8_t RIGHT_DRIVE_1_PORT = 13;
int8_t RIGHT_DRIVE_2_PORT = 14;
int8_t RIGHT_DRIVE_3_PORT = 3;
int8_t RIGHT_DRIVE_4_PORT = 4;

Motor leftRoller(LEFT_ROLLER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor rightRoller(RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

Motor leftLift(LEFT_LIFT_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rightLift(RIGHT_LIFT_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor trayMotor(TILTER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);


Controller joystick;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);

ControllerButton liftUp(ControllerDigital::up);
ControllerButton liftDown(ControllerDigital::down);

ControllerButton trayDown(ControllerDigital::R1);
ControllerButton trayUp(ControllerDigital::R2);

ControllerButton presetX(ControllerDigital::X);
ControllerButton presetB(ControllerDigital::B);

ControllerButton slowRollBtn(ControllerDigital::Y);


// auto chassis = ChassisControllerBuilder()
// 	.withMotors(
// 		{LEFT_DRIVE_1_PORT, static_cast<int8_t>(-LEFT_DRIVE_2_PORT), static_cast<int8_t>(-LEFT_DRIVE_3_PORT), LEFT_DRIVE_4_PORT},
// 		{static_cast<int8_t>(-RIGHT_DRIVE_1_PORT), RIGHT_DRIVE_2_PORT, RIGHT_DRIVE_3_PORT, static_cast<int8_t>(-RIGHT_DRIVE_4_PORT)}
// 	).withDimensions(
// 		AbstractMotor::gearset::green,
// 		{
// 			{4_in, 16_in},
// 			imev5GreenTPR
// 		}
// 	).withOdometry().buildOdometry();

auto chassis = ChassisControllerBuilder()
	.withMotors(
		{LEFT_DRIVE_1_PORT, static_cast<int8_t>(-LEFT_DRIVE_2_PORT), static_cast<int8_t>(-LEFT_DRIVE_3_PORT), LEFT_DRIVE_4_PORT},
		{static_cast<int8_t>(-RIGHT_DRIVE_1_PORT), RIGHT_DRIVE_2_PORT, RIGHT_DRIVE_3_PORT, static_cast<int8_t>(-RIGHT_DRIVE_4_PORT)}
	).withDimensions(
		AbstractMotor::gearset::green,
		{
			{4_in, 16_in},
			imev5GreenTPR
		}
	).build();



void on_center_button() {}


void initialize() {
	leftLift.setBrakeMode(AbstractMotor::brakeMode::hold);
	rightLift.setBrakeMode(AbstractMotor::brakeMode::hold);
	trayMotor.setBrakeMode(AbstractMotor::brakeMode::hold);

	leftLift.tarePosition();
	rightLift.tarePosition();

	// chassis->setState({0_in, 0_in, 0_deg});

	pros::lcd::initialize();
}


void disabled() {}
void competition_initialize() {}


/**
 * Moves both roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void rollers(int speed) {
	leftRoller.moveVelocity(-speed);
	rightRoller.moveVelocity(speed);
}


/**
 * Moves the roller lift. Speed will depend on the speed parameter. The
 * range is -100 to 100.
*/
void lift(int speed) {
	leftLift.moveVelocity(speed * 2);
	rightLift.moveVelocity(-speed * 2);
}


/**
 * Moves the roller lift to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void liftPosition(int pos, int speed) {
	leftLift.moveAbsolute(pos, speed);
	rightLift.moveAbsolute(-pos, speed);
}


/**
 * Moves the tray motor. Speed will depend on the speed parameter. The
 * range is -100 to 100. If speed is 0, the motor will stop with a
 * brakeType of "hold."
*/
void tilter(int speed) {
	trayMotor.moveVelocity(speed);
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
	} else if (slowRollBtn.isPressed()) {
		rollers(25);
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

	int diff = abs(leftLift.getPosition()) - abs(rightLift.getPosition());

	if (abs(diff) > 90) {
		int direction = leftLift.getDirection();

		int reset_speed = 80;
		if (direction == 1) {
			if (diff > 0) {
				rightLift.moveVelocity(0);
				leftLift.moveAbsolute(-rightLift.getPosition(), reset_speed);
			} else {
				leftLift.moveVelocity(0);
				rightLift.moveAbsolute(-leftLift.getPosition(), reset_speed);
			}
		} else {
			if (diff > 0) {
				rightLift.moveVelocity(0);
				leftLift.moveAbsolute(-rightLift.getPosition(), reset_speed);
			} else {
				leftLift.moveVelocity(0);
				rightLift.moveAbsolute(-leftLift.getPosition(), reset_speed);
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
		tilter(80);
	} else if (trayDown.isPressed()) {
		tilter(-65);
	} else {
		tilter(0);
	}
}


// void forward(QLength x, QLength y, int speed) {
// 	chassis->setMaxVelocity(speed * 2);
// 	chassis->driveToPoint({x, y});
// 	chassis->setMaxVelocity(200);
// 	chassis->setState({0_in, 0_in, 0_deg});
// }
//
// void backward(QLength x, QLength y, int speed) {
// 	chassis->setMaxVelocity(speed * 2);
// 	chassis->driveToPoint({x, y}, true);
// 	chassis->setMaxVelocity(200);
// 	chassis->setState({0_in, 0_in, 0_deg});
// }
//
// void turn(QAngle angle, int speed) {
// 	chassis->setMaxVelocity(speed * 2);
// 	chassis->turnToAngle(angle);
// 	chassis->setMaxVelocity(200);
// 	chassis->setState({0_in, 0_in, 0_deg});
// }
//

void autonomous() {
	// forward(0.5_ft, 0_ft, 15);
	// rollers(-90);
	// forward(1.4_ft, 0_ft, 12);
	// rollers(0);
	// backward(-1.0_ft, 0_ft, 15);

	chassis->setMaxVelocity(30);
	chassis->moveDistanceAsync(0.5_ft);
	chassis->waitUntilSettled();
	rollers(-100);
	chassis->setMaxVelocity(24);
	chassis->moveDistanceAsync(1.5_ft);
	chassis->waitUntilSettled();
	rollers(0);
	chassis->setMaxVelocity(35);
	chassis->moveDistance(-0.6_ft);
	chassis->turnAngle(28_deg);
	chassis->moveDistance(-1.8_ft);
	chassis->turnAngle(-36_deg);
	chassis->setMaxVelocity(30);
	rollers(-100);
	chassis->setMaxVelocity(24);
	chassis->moveDistanceAsync(1.5_ft);
	chassis->waitUntilSettled();
	rollers(0);
	chassis->moveDistance(-1.0_ft);
	chassis->turnAngle(-135_deg);

}


void opcontrol() {
	while(true) {
		chassis->getModel()->arcade(
			joystick.getAnalog(ControllerAnalog::leftY),
			joystick.getAnalog(ControllerAnalog::rightX)
		);

		rollersControl();
		liftControl();
		tilterControl();
		liftPresets();

		pros::delay(20);
	}
}
