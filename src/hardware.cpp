#include "hardware.hpp"
#include "pros/adi.hpp"

// Motor groups
pros::MotorGroup left_motors({-1, -2, -3,4,5,6}, pros::MotorGearset::blue);
pros::MotorGroup right_motors({7, 8, 9,10,11,12}, pros::MotorGearset::blue);
pros::Motor      primary_intake(50, pros::MotorGearset::blue);            // Primary Intake on port 5
pros::Motor      secondary_intake(50, pros::MotorGearset::blue);          // Secondary Intake on port 4

// Controller and Pistons
pros::Controller      controller(pros::E_CONTROLLER_MASTER); // Initialize controller
pros::adi::DigitalOut pistonMogo('A');                       // Initialize the solenoid for MOGO mech
pros::adi::DigitalOut pistonDoink('B');                               // Initalize the Doinker
pros::adi::DigitalOut pistonLB('C');                         // Initialize the LB mech

// Sensors
pros::Imu             imu(50); // IMU
pros::Rotation        vertical_encoder(50);
pros::Rotation        horizontal_encoder(50);
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_2, 3);
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_2, 0);

// Drivetrain settings
lemlib::Drivetrain  drivetrain(&left_motors,               // left motor group
                               &right_motors,              // right motor group
                               16,                         // 10 inch track width
                               lemlib::Omniwheel::NEW_325, // using new 4" omnis
                               600,                        // drivetrain rpm is 360
                               2                           // horizontal drift is 2 (for now)
 );
lemlib::OdomSensors sensors(&vertical_tracking_wheel,   // Vertical tracking wheel
                            nullptr,                    // 2nd Vertical Tracking Wheel
                            &horizontal_tracking_wheel, // Horizontal Tracking wheel
                            nullptr,                    // 2nd Horizontal tracking wheel
                            &imu                        // IMU Sensor
);

// PID controllers
lemlib::ControllerSettings angular_controller(10,  // proportional gain (kP) 19
                                              0.1,  // integral gain (kI)
                                              1, // derivative gain (kD) 15
                                              3,  // anti windup
                                              0.1,  // small error range, in inches
                                              100,  // small error range timeout, in milliseconds
                                              1,  // large error range, in inches
                                              500,  // large error range timeout, in milliseconds
                                              0   // maximum acceleration (slew)
);
lemlib::ControllerSettings lateral_controller(0, // proportional gain (kP)
                                              0,  // integral gain (kI)
                                              0,  // derivative gain (kD)
                                              0,  // anti windup
                                              0,  // small error range, in inches
                                              0,  // small error range timeout, in milliseconds
                                              0,  // large error range, in inches
                                              0,  // large error range timeout, in milliseconds
                                              0   // maximum acceleration (slew)
);

// Input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3,    // joystick deadband out of 127
                                      10,   // minimum output where drivetrain will move out of 127
                                      1.019 // expo curve gain
);

// Input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3,    // joystick deadband out of 127
                                   10,   // minimum output where drivetrain will move out of 127
                                   1.019 // expo curve gain
);

// Create the chassis
lemlib::Chassis chassis(drivetrain,
                        lateral_controller,
                        angular_controller,
                        sensors,
                        &throttle_curve,
                        &steer_curve);