#pragma once
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\sensors\ISensor.h"
#include <random>

class Gyroscope : public ISensor {
    std::default_random_engine engine;
    std::normal_distribution<double> angleDist;
public:
    Gyroscope() : angleDist(0.0, 1.0) {}
    SensorEvent read() override {
        return {"Gyroscope", angleDist(engine), currentTimeMillis()};
    }
    std::string getType() const override { return "Gyroscope"; }
};