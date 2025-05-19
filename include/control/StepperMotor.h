#pragma once

class StepperMotor {
public:
    StepperMotor(int stepsPerRevolution = 200);

    void stepForward(int steps);
    void stepBackward(int steps);
    void reset();

    int getCurrentStep() const;

private:
    int currentStep;
    int stepsPerRevolution;
};
