#pragma once
#include "control/StepperMotor.h"
#define _USE_MATH_DEFINES
#include <cmath> 

class MotorControl {

public:
    MotorControl();
    ~MotorControl();

    void moveForward(double distance);
    void moveBackward(double distance);
    void rotate(double angle);
    int getTotalSteps();
    void resetTotalSteps();

private:
    StepperMotor* leftMotor;
    StepperMotor* rightMotor;

    DWORD threadIdLeftMotor;
    HANDLE hThreadLeftMotor;

    DWORD threadIdRightMotor;
    HANDLE hThreadRightMotor;

    double wheelRadius;
    double wheelBase;
    int stepsPerRevolution;

    int distanceToSteps(double distance);
    int angleToSteps(double angle);
};
