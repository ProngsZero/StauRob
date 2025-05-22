#include "core/StauRob.h"

#ifdef USELOG
StauRob::StauRob(std::shared_ptr<spdlog::logger> logger)
    :   state("init"),
        localizer(sensors, &state, &stateMutex), // Pass vector of raw pointers
        motorControl(),
        mapper(&localizer, &state, &stateMutex),
        running(false),
        logger(logger)
{
    // Create sensors and add raw pointers to the vector
    sensors.push_back(new ProximitySensor()); //sensors[0]
    sensors.push_back(new Gyroscope()); //sensors[1]
    sensors.push_back(new BumperSensor()); //sensors[2]
    sensors.push_back(new CliffSensor()); //sensors[3]
    for (size_t i = 0; i < sensors.size(); ++i) {
        LOG_INFO("Sensor {}: {}", i, sensors[i]->getType());
    }

    LOG_INFO("Started Thread Mapper");
    mapperThread = std::thread([this]() {
        while (running) {
            mapper.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    LOG_INFO("Started Thread Localizer");
    localizerThread = std::thread([this]() {
        while (running) {
            localizer.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}


#else
StauRob::StauRob()
    : state("init"),
    motorControl(),
    sr_running(false)
{
    // Create sensors and add raw pointers to the vector
    sensors.push_back(new ProximitySensor()); //sensors[0]
    sensors.push_back(new Gyroscope()); //sensors[1]
    sensors.push_back(new BumperSensor()); //sensors[2]
    sensors.push_back(new CliffSensor()); //sensors[3]
    InitializeCriticalSection(&stateMutex);
    InitializeCriticalSection(&updateMutex);
    localizer = new Localizer(sensors, &state, &stateMutex, &updateMutex);
    mapper = new Mapper(localizer, &state, &stateMutex, &updateMutex);
}
#endif

void StauRob::setValue(int index, bool value)
{
    if(index < sensors.size() && index > 0)
    {
        sensors[index]->setValue(value);
        LOG_INFO("Value change on {}: {}", sensors[index]->getType(), value);
    }
    else {
        LOG_INFO("Called setValue but did not execute");
    }
}

StauRob::~StauRob() {
    for (auto* sensor : sensors) {
        LOG_INFO("Deleting sensor {}", sensor->getType());
        delete sensor;
    }
}

void StauRob::stop() {
    LOG_INFO("Calling stop of StauRob");
    sr_running = false;
    if (hThreadLocalizer != NULL)
    {
        WaitForSingleObject(hThreadLocalizer, INFINITE);
        std::cout << "joined thread 1" << std::endl;
        CloseHandle(hThreadLocalizer);
        hThreadLocalizer = NULL;
    }

    if (hThreadMapper != NULL)
    {
        WaitForSingleObject(hThreadMapper, INFINITE);
        std::cout << "joined thread 2" << std::endl;
        CloseHandle(hThreadMapper);
        hThreadMapper = NULL;
    }
    DeleteCriticalSection(&stateMutex);
    DeleteCriticalSection(&updateMutex);
}

void StauRob::run() {
    std::cout << "StauRob is starting...\n";
    LOG_INFO("Calling run of StauRob");
    sr_running = true;
    SensorEvent sensEvnt;

    hThreadMapper = CreateThread(
        NULL,             
        0,               
        StauRob::mapperThreadFunc,
        this,             
        0,                
        &threadIdMapper         
    );
    hThreadLocalizer = CreateThread(
        NULL,             
        0,                
        StauRob::localizerThreadFunc, 
        this,             
        0,                
        &threadIdLocalizer         
    );

    // Poll sensors
    for (auto* sensor : sensors) {
        sensEvnt = sensor->read();
        std::cout << sensEvnt.toString() << std::endl;
        if(sensEvnt.type == "Bumper" && sensEvnt.value == 1.0)
        {
            this->stop();
            std::cout << "stopped due to bumper sensor" << std::endl;
        }
            
        if(sensEvnt.type == "Cliff" && sensEvnt.value == 1.0)
        {
            this->stop();
            std::cout << "stopped due to cliff sensor" << std::endl;
        }
        LOG_INFO("Executing sensor polling");
    }

    // Example movement
    motorControl.moveForward(10.0);
    motorControl.rotate(90.0);

    std::cout << "Run complete.\n";
}

void StauRob::setState(std::string newState) 
{
    state = newState;
}

std::string StauRob::getState(void)
{
    return state;
}

DWORD WINAPI StauRob::localizerThreadFunc(LPVOID lpParam)
{
    StauRob* self = static_cast<StauRob*>(lpParam);
    while (self->sr_running) {
        std::cout << "localizer thread " << self->getState() << std::endl;
        EnterCriticalSection(&self->updateMutex);
        self->localizer->update();
        EnterCriticalSection(&self->stateMutex);
        self->setState("localizing");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        LeaveCriticalSection(&self->stateMutex);
        LeaveCriticalSection(&self->updateMutex);
    }
    return 0;
}

DWORD WINAPI StauRob::mapperThreadFunc(LPVOID lpParam)
{
    StauRob* self = static_cast<StauRob*>(lpParam);
    while (self->sr_running) {
        std::cout << "mapper thread " << self->getState() << std::endl;
        EnterCriticalSection(&self->stateMutex);
        self->setState("mapping");
        EnterCriticalSection(&self->updateMutex);
        self->mapper->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        LeaveCriticalSection(&self->updateMutex);
        LeaveCriticalSection(&self->stateMutex);
    }
    return 0;
}