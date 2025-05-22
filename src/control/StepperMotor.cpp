#include "control/StepperMotor.h"

StepperMotor::StepperMotor(int stepsPerRevolution)
    : currentStep(0), stepsPerRevolution(stepsPerRevolution) {}

void StepperMotor::stepForward(int steps) {
    currentStep += steps;
}

void StepperMotor::stepBackward(int steps) {
    currentStep -= steps;
}

void StepperMotor::reset() {
    currentStep = 0;
}

int StepperMotor::getCurrentStep() const {
    return currentStep;
}
