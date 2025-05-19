#pragma once
#include <string>
#include <cstdint>
#include <sstream>
#include <chrono>

struct SensorEvent {
    std::string type;
    double value;
    uint64_t timestamp;

    std::string toString() const {
        std::ostringstream oss;
        oss << "SensorEvent { "
            << "type: \"" << type << "\", "
            << "value: " << value << ", "
            << "timestamp: " << timestamp
            << " }";
        return oss.str();
    }
};

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual SensorEvent read() = 0;
    virtual std::string getType() const = 0;
    virtual void setValue(bool value) {}

    static uint64_t currentTimeMillis() {
        return static_cast<uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count()
        );
    }
};