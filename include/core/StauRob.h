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

#ifdef USELOG
#include <spdlog/logger.h>
#define LOG_INFO(...) logger->info(__VA_ARGS__)
#else
#define LOG_INFO(...) // logging disabled
#endif

class StauRob {

private:
    std::vector<ISensor*> sensors;
    std::string state;
    Mapper* mapper;
    Localizer* localizer;
    MotorControl motorControl;
    Pair position;

    DWORD threadIdMapper;
    HANDLE hThreadMapper;

    DWORD threadIdLocalizer;
    HANDLE hThreadLocalizer;

    #ifdef USELOG
    std::shared_ptr<spdlog::logger> logger;
    #endif
    bool sr_running;
    CRITICAL_SECTION stateMutex;
    CRITICAL_SECTION updateMutex;

public:
    void setValue(int index, bool value);
    int cycles;
    #ifdef USELOG
    StauRob(std::shared_ptr<spdlog::logger> logger);
    #else
    StauRob();
    #endif

    ~StauRob();
    void run();
    void stop();
    std::string getState();
    void setState(std::string newState);
    static DWORD WINAPI mapperThreadFunc(LPVOID lpParam);
    static DWORD WINAPI localizerThreadFunc(LPVOID lpParam);
};
