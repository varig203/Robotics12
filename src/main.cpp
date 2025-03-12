#include "main.h"

// 12 motor drive
void chassis_fn() {
    while (true) {
        int leftY  = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);  // Up and down on the left stick
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // Left and right on the right stick

        // Drivetrain control
        chassis.arcade(leftY, rightX);
        pros::delay(20);
    }
}

// Runs initialization code when the program starts; all other competition modes are blocked, keep exec under few seconds
void initialize() {
    chassis.calibrate(); // calibrate sensors
}

// Runs while the robot is disabled, following autonomous or opcontrol, and exits when the robot is enabled.
void disabled() {
    controller.print(0, 0, "Emergency initiated"); // incase the driver can't see the warning
    controller.rumble(".-");           // Non-verbal warning to driver

	const int MAX_ARRAY_SIZE = 1000000;
	int bigArray[MAX_ARRAY_SIZE];  // Declare the array before using it

	while (true) { // Emergency stop 
		right_motors.move_velocity(0);
		left_motors.move_velocity(0);
		left_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		right_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		// for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        //     bigArray[i] = i;  // Filling the array with values, the brain will eventually crash
        // }
		pros::delay(10);
	}
}

void competition_initialize() {} // not for comp
void autonomous() {} // not for comp

// Runs the operator control code in its own task when the robot is enabled, stops if disabled or comms lost.
void opcontrol() {
	bool enable = false;
    // These just run the functions on seperate threads for async
    pros::Task chassisControl(chassis_fn);
//    pros::Task solenoidControl(solenoidControl_fn);
//    pros::Task motorControl(motorControl_fn);
	
	// Sensor reading
	while (true) {
		// Emergency stop mechanism
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
			if (enable) {
				disabled();
				enable = !enable;
			} else {
				enable = !enable;
			}
		}
		// Checking rpm of motors
		if (right_motors.get_actual_velocity() == 600 || left_motors.get_actual_velocity() == 600) {
			controller.print(0, 0, "MOTORS FULL POWER");
		} else {
			controller.clear_line(0);
		}
		pros::delay(20);
	}
}