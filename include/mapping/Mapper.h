#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include "mapping/Localizer.h"

class Mapper {
private:
    int mapId;

    std::unordered_map<Pair, std::string> map;

    Localizer* localizer;
    CRITICAL_SECTION* stateMutex;
    CRITICAL_SECTION* updateMutex;
    char* stateString;

public:
    Mapper(Localizer* loc, char* state, CRITICAL_SECTION* mutex, CRITICAL_SECTION* updateMutex);
    void setCell(Pair pair, const std::string& state);

    void save(const std::string& filename);
    void load(const std::string& filename);

    std::vector<Pair> calculatePath(const Pair& start, const Pair& end);
    void update();
};