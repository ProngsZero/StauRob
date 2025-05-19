#pragma once
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\sensors\ISensor.h"
#include <vector>

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
    double y;
    double orientation; // In degrees
};

class Localizer {
    std::vector<ISensor*> sensors;
    Position currentPos{0.0, 0.0, 0.0};
public:
    Localizer(const std::vector<ISensor*>& sens) : sensors(sens) {}
    
    void update();
    Pair getPosition() const;
    Position getOrientation() const;
};