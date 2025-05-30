#pragma once
#include <Windows.h>

extern int totalSteps;

class StepperMotor {

public:
    StepperMotor(int stepsPerRevolution);

    static DWORD WINAPI update(LPVOID lpParam);
    void stepForward(int steps);
    void stepBackward(int steps);
    void setActive();
    void setInactive();
    void reset();
private:
    bool active;
    int forwardSteps;
    int backwardSteps;
    int stepsPerRevolution;
};
