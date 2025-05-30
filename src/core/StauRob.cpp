#include "core/StauRob.h"

StauRob::StauRob(ISensor* sensors[], char* nameIn)
{
    setState("init");
    strcpy(name, nameIn);
    bufferIndex = 0;
    motorControl = new MotorControl();
    memcpy(&sensorsArr, &sensors, 4);

    InitializeCriticalSection(&stateMutex);
    InitializeCriticalSection(&updateMutex);

    localizer = new Localizer(sensorsArr, state, &stateMutex, &updateMutex);
    mapper = new Mapper(localizer, state, &stateMutex, &updateMutex);
    sr_running = true;

    if (!hThreadMapper)
    {
        hThreadMapper = CreateThread(
            NULL,
            0,
            StauRob::mapperThreadFunc,
            this,
            0,
            &threadIdMapper
        );
    }
    if (!hThreadLocalizer)
    {
        hThreadLocalizer = CreateThread(
            NULL,
            0,
            StauRob::localizerThreadFunc,
            this,
            0,
            &threadIdLocalizer
        );
    }
}

void StauRob::setValue(int index, bool value)
{
    if(index < 4 && index > 0)
    {
        sensorsArr[index]->setValue(value);
    }
}

StauRob::~StauRob() 
{
    for (auto* sensor : sensorsArr)
    {
        delete sensor;
    }
    delete motorControl;
    delete localizer;
    delete mapper;
}

void StauRob::stop() {
    sr_running = false;
    DWORD ret;
    if (hThreadLocalizer != NULL)
    {
        ret = WaitForSingleObject(hThreadLocalizer, INFINITE);
        if (ret == WAIT_FAILED)
            std::cout << "Thread join failed" << std::endl;
        else
            std::cout << "joined thread localizer" << std::endl;
        CloseHandle(hThreadLocalizer);
        hThreadLocalizer = NULL;
    }

    if (hThreadMapper != NULL)
    {
        ret = WaitForSingleObject(hThreadMapper, INFINITE);
        if (ret == WAIT_FAILED)
            std::cout << "Thread join failed" << std::endl;
        else
            std::cout << "joined thread mapper" << std::endl;
        CloseHandle(hThreadMapper);
        hThreadMapper = NULL;
    }
    DeleteCriticalSection(&stateMutex);
    DeleteCriticalSection(&updateMutex);
}

void StauRob::checkSensors() 
{
    SensorEvent sensEvnt;
        // Poll sensors

    for (int i = 0; i < 5; i++)
    {
        sensEvnt = sensorsArr[i]->read();

        if (sensEvnt.type == "Bumper" && sensEvnt.value == 1.0)
        {
            this->stop();
            std::cout << "stopped due to bumper sensor" << std::endl;
        }

        if (sensEvnt.type == "Cliff" && sensEvnt.value == 1.0)
        {
            this->stop();
            std::cout << "stopped due to cliff sensor" << std::endl;
        }
    }
}

void StauRob::move(double dist)
{
    if (dist > 0.0)
    {
        motorControl->moveForward(dist);
    }
    else
    {
        motorControl->moveBackward(dist);
    }
}

void StauRob::printStepsTaken()
{
    std::cout << "[Command] Amount of steps done: " << motorControl->getTotalSteps() << std::endl;
    motorControl->resetTotalSteps();
}

void StauRob::setState(const char* newState) 
{
    strcpy(state, newState);
}

char* StauRob::getState(void)
{
    return state;
}


DWORD WINAPI StauRob::localizerThreadFunc(LPVOID lpParam)
{
    StauRob* self = static_cast<StauRob*>(lpParam);

    while (self->sr_running) 
    {
        //std::cout << "localizer thread " << self->getState() << std::endl;
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

    while (self->sr_running) 
    {
        //std::cout << "mapper thread " << self->getState() << std::endl;
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

void StauRob::setName(const char* nameIn)
{
    strcpy(name, nameIn);
}