#pragma once
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\sensors\ISensor.h"

class BumperSensor : public ISensor {
    bool bump;
public:
    BumperSensor() : bump(false) {};
    SensorEvent read() override {
        return {"Bumper", bump ? 1.0 : 0.0, currentTimeMillis()}; // 0 = no collision, 1 = collision
    }
    std::string getType() const override { return "Bumper"; }

    void setValue(bool value) override {
        bump = value;
    }
};