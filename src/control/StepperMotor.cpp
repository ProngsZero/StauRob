#pragma once
#include "control/StepperMotor.h"

int totalSteps = 0;


StepperMotor::StepperMotor(int stepsPerRevolution)
    : forwardSteps(0), backwardSteps(0), active(true), stepsPerRevolution(stepsPerRevolution) {}

DWORD WINAPI StepperMotor::update(LPVOID lpParam)
{
    StepperMotor* motor = static_cast<StepperMotor*>(lpParam);
    while (motor->active)
    {
        if (motor->forwardSteps > 0)
        {
            motor->forwardSteps--;
            totalSteps++;
        }
        if (motor->backwardSteps > 0)
        {
            motor->backwardSteps--;
            totalSteps++;
        }
    }
    return 0;
}

void StepperMotor::stepForward(int steps) 
{
    forwardSteps += steps;
}

void StepperMotor::stepBackward(int steps) 
{
    backwardSteps -= steps;
}

void StepperMotor::reset() 
{
    backwardSteps = 0;
    forwardSteps = 0;
}

void StepperMotor::setActive()
{
    active = true;
}

void StepperMotor::setInactive()
{
    active = false;
}