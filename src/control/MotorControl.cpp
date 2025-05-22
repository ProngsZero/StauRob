#include "control/MotorControl.h"

MotorControl::MotorControl()
    : wheelRadius(0.03),    // 3 cm radius
      wheelBase(0.15),      // 15 cm distance between wheels
      stepsPerRevolution(200),
      leftMotor(200), 
      rightMotor(200) {}

void MotorControl::moveForward(double distance) {
    int steps = distanceToSteps(distance);
    leftMotor.stepForward(steps);
    rightMotor.stepForward(steps);
}

void MotorControl::moveBackward(double distance) {
    int steps = distanceToSteps(distance);
    leftMotor.stepBackward(steps);
    rightMotor.stepBackward(steps);
}

void MotorControl::rotate(double angle) {
    int steps = angleToSteps(angle);
    if (angle > 0) {
        leftMotor.stepForward(steps);
        rightMotor.stepBackward(steps);
    } else {
        leftMotor.stepBackward(steps);
        rightMotor.stepForward(steps);
    }
}

int MotorControl::distanceToSteps(double distance) {
    double wheelCircumference = 2 * M_PI * wheelRadius;
    double revolutions = distance / wheelCircumference;
    return static_cast<int>(revolutions * stepsPerRevolution);
}

int MotorControl::angleToSteps(double angle) {
    double rotationCircumference = M_PI * wheelBase;
    double distanceForRotation = (angle / 360.0) * rotationCircumference;
    return distanceToSteps(distanceForRotation);
}
