#pragma once
#include <vector>
#include <iostream>
//#include <thread>
#include <Windows.h>


#include "control/MotorControl.h"
#include "mapping/Localizer.h"
#include "mapping/Mapper.h"
#include "sensors/ISensor.h"
#include "sensors/ProximitySensor.h"
#include "sensors/BumperSensor.h"
#include "sensors/GyroscopeSensor.h"
#include "sensors/CliffSensor.h"

#define BUFFERSIZE 10

class StauRob {

private:
    ISensor* sensorsArr[4];
    char state[20];
    Mapper* mapper;
    Localizer* localizer;
    MotorControl* motorControl;
    Pair position;

    SensorEvent events[BUFFERSIZE];
    int bufferIndex;

    DWORD threadIdMapper;
    HANDLE hThreadMapper;

    DWORD threadIdLocalizer;
    HANDLE hThreadLocalizer;

    bool sr_running;
    CRITICAL_SECTION stateMutex;
    CRITICAL_SECTION updateMutex;

public:
    char name[20];
    void setValue(int index, bool value);
    StauRob(ISensor* sensorArr[], char* nameIn);
    ~StauRob();

    void printStepsTaken();
    void move(double distance);
    void clearSteps();
    void setName(const char* name);

    void checkSensors();
    void stop();
    char* getState();
    void setState(const char* newState);
    static DWORD WINAPI mapperThreadFunc(LPVOID lpParam);
    static DWORD WINAPI localizerThreadFunc(LPVOID lpParam);
};
