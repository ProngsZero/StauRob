#include "control/StepperMotor.h"
#define _USE_MATH_DEFINES
#include <cmath> 

class MotorControl {
public:
    MotorControl();

    void moveForward(double distance);
    void moveBackward(double distance);
    void rotate(double angle);

private:
    StepperMotor leftMotor;
    StepperMotor rightMotor;

    double wheelRadius;
    double wheelBase;
    int stepsPerRevolution;

    int distanceToSteps(double distance);
    int angleToSteps(double angle);
};
