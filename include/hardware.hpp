#pragma once

#include "lemlib/api.hpp" // IWYU pragma: keep
#include "api.h"

// Motor groups
extern pros::MotorGroup left_motors;
extern pros::MotorGroup right_motors;

// Controllers and Pistons
extern pros::Controller      controller;
// Sensors
extern pros::Imu             imu;

// Drivetrain settings
extern lemlib::Drivetrain  drivetrain;
extern lemlib::OdomSensors sensors;

// PID controllers
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;

// Input curves
extern lemlib::ExpoDriveCurve throttle_curve;
extern lemlib::ExpoDriveCurve steer_curve;

// Chassis
extern lemlib::Chassis chassis;

void initialize_hardware();