#pragma once
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\sensors\ISensor.h"
#include <random>

class ProximitySensor : public ISensor {
    std::default_random_engine engine;
    std::normal_distribution<double> dist;
public:
    ProximitySensor() : dist(100.0, 5.0) {}
    SensorEvent read() override {
        return {"Distance", dist(engine), currentTimeMillis()};
    }
    std::string getType() const override { return "Distance"; }
};