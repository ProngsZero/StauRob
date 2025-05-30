#include "mapping/Localizer.h"

Localizer::Localizer(ISensor* senrs[], char* state, CRITICAL_SECTION* mutex, CRITICAL_SECTION* updateMutex)
{
    memcpy(&sensorArr, &senrs, 4);
    this->stateString = state;
    this->updateMutex = updateMutex;
    this->stateMutex = mutex;
}

void Localizer::update()
{
    for (auto* sensor : sensorArr)
    {
        if (sensor->getType() == "Gyroscope")
        {
            currentPos.orientation += sensor->read().value;
        }
        // Optionally handle more sensors for x/y movement
    }
}

Position Localizer::getOrientation() const 
{
    return currentPos;
}

Pair Localizer::getPosition() const 
{
    return Pair(currentPos.x, currentPos.y); 
}