#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "mapping/Localizer.h"

class Mapper {
private:
    std::unordered_map<Pair, std::string> map;
    int mapId;
    Localizer* localizer = nullptr;
    CRITICAL_SECTION* stateMutex;
    CRITICAL_SECTION* updateMutex;
    std::string* stateString;
    //TODO event queue

public:
    Mapper(Localizer* loc, std::string* state, CRITICAL_SECTION* mutex, CRITICAL_SECTION* updateMutex) :
        localizer(loc), stateMutex(mutex), stateString(state), updateMutex(updateMutex) {}
    void setCell(Pair pair, const std::string& state);

    void save(const std::string& filename);
    void load(const std::string& filename);

    std::vector<Pair> calculatePath(const Pair& start, const Pair& end);
    void update(); //TODO, read event queue
};