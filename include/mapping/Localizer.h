#pragma once
#include "sensors/ISensor.h"
#include <Windows.h>
#include <vector>
#include <mutex>

struct Pair {
    double x, y;
    bool operator==(const Pair& other) const { return x == other.x && y == other.y; }
    Pair(double valX, double valY) : x(valX), y(valY) {};
    Pair() : x(0.0), y(0.0) {};
};

namespace std {
    template<>
    struct hash<Pair> {
        size_t operator()(const Pair& p) const {
            return hash<double>()(p.x) ^ (hash<double>()(p.y) << 1);
        }
    };
}

struct Position {
    double x;
    bool is_valid;
    double y;
    short alorithmNumber;
    double orientation;
};

class Localizer {
private:
    ISensor* sensorArr[4];
    Position currentPos{0.0, 0.0, 0.0};
    CRITICAL_SECTION* stateMutex;
    CRITICAL_SECTION* updateMutex;
    char* stateString;
public:
    Localizer(ISensor* senrs[], char* state, CRITICAL_SECTION* mutex, CRITICAL_SECTION* updateMutex);

    void update();
    Pair getPosition() const;
    Position getOrientation() const;
};