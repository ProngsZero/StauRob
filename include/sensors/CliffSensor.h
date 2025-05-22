#pragma once
#include "sensors/ISensor.h"

class CliffSensor : public ISensor {
    bool cliff;
public:
    CliffSensor() : cliff(false) {};
    SensorEvent read() override {
        return {"Cliff", cliff ? 1.0 : 0.0, currentTimeMillis()}; // 0 = safe, 1 = cliff detected
    }
    std::string getType() const override { return "Cliff"; }

    void setValue(bool value) override {
        cliff = value;
    }
};
