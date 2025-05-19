#pragma once
#include <vector>
#include <iostream>
#include <thread>

#include <spdlog/logger.h>
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\control\MotorControl.h"
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\mapping\Localizer.h"
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\mapping\Mapper.h"
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\sensors\ISensor.h"
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\sensors\ProximitySensor.h"
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\sensors\BumperSensor.h"
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\sensors\GyroscopeSensor.h"
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\sensors\CliffSensor.h"

class StauRob {
    std::vector<ISensor*> sensors;
    Mapper mapper;
    Localizer localizer;
    MotorControl motorControl;
    Pair position;
    std::thread mapperThread;
    std::thread localizerThread;
    std::shared_ptr<spdlog::logger> logger;
    bool running;

public:
    void setValue(int index, bool value);
    StauRob(std::shared_ptr<spdlog::logger> logger);
    ~StauRob();
    void run();
    void stop();
};
