#pragma once
#include "control/MotorControl.h"
#include <iostream>

MotorControl::MotorControl()
    : wheelRadius(0.03),    // 3 cm radius
      wheelBase(0.15),      // 15 cm distance between wheels
      stepsPerRevolution(200)
{
    leftMotor = new StepperMotor(200);
    rightMotor = new StepperMotor(200);
    std::cout << "Starting left motor" << std::endl;

    hThreadLeftMotor = CreateThread(
        NULL,
        0,
        StepperMotor::update,
        leftMotor,
        0,
        &threadIdLeftMotor
    );

    std::cout << "Starting right motor" << std::endl;
    hThreadRightMotor = CreateThread(
        NULL,
        0,
        StepperMotor::update,
        rightMotor,
        0,
        &threadIdRightMotor
    );
}

MotorControl::~MotorControl() 
{
    // Signal threads to stop
    leftMotor->setInactive();
    rightMotor->setInactive();
    DWORD ret;
    // Wait for threads to finish
    if (hThreadLeftMotor) 
    {
        ret = WaitForSingleObject(hThreadLeftMotor, INFINITE);
        if (ret == WAIT_FAILED)
            std::cout << "Thread join failed" << std::endl;
        else
            std::cout << "closed thread leftMotor" << std::endl;
        CloseHandle(hThreadLeftMotor);
    }

    if (hThreadRightMotor) 
    {
        ret = WaitForSingleObject(hThreadRightMotor, INFINITE);
        if (ret == WAIT_FAILED)
            std::cout << "Thread join failed" << std::endl;
        else
            std::cout << "closed thread rightMotor" << std::endl;
        CloseHandle(hThreadRightMotor);
    }
    
    delete leftMotor;
    delete rightMotor;
}

int MotorControl::getTotalSteps()
{
    //std::cout << "returining TotalSteps " << totalSteps << std::endl;
    return totalSteps;
}

void MotorControl::resetTotalSteps()
{
    totalSteps = 0;
}

void MotorControl::moveForward(double distance) 
{
    int steps = distanceToSteps(distance);
    std::cout << "Each motor will move forward by " << steps << " steps." << std::endl;
    leftMotor->stepForward(steps);
    rightMotor->stepForward(steps);
}

void MotorControl::moveBackward(double distance) 
{
    int steps = distanceToSteps(distance);
    std::cout << "Each motor will move backward by " << steps << " steps." << std::endl;
    leftMotor->stepBackward(steps);
    rightMotor->stepBackward(steps);
}

void MotorControl::rotate(double angle) 
{
    int steps = angleToSteps(angle);
    if (angle > 0) 
    {
        leftMotor->stepForward(steps);
        rightMotor->stepBackward(steps);
    } else 
    {
        leftMotor->stepBackward(steps);
        rightMotor->stepForward(steps);
    }
}

int MotorControl::distanceToSteps(double distance) 
{
    double wheelCircumference = 2 * M_PI * wheelRadius;
    double revolutions = distance / wheelCircumference;
    return static_cast<int>(revolutions * stepsPerRevolution);
}

int MotorControl::angleToSteps(double angle) 
{
    double rotationCircumference = M_PI * wheelBase;
    double distanceForRotation = (angle / 360.0) * rotationCircumference;
    return distanceToSteps(distanceForRotation);
}
