#include "mapping/Localizer.h"

void Localizer::update() {

    for (auto* sensor : sensors) {
        if (sensor->getType() == "Gyroscope") {
            currentPos.orientation += sensor->read().value;
        }
        // Optionally handle more sensors for x/y movement
    }
}

Position Localizer::getOrientation() const {
    return currentPos;
}

Pair Localizer::getPosition() const {
    return Pair(currentPos.x, currentPos.y); 
}