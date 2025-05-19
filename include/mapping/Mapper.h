#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "C:\Users\artur.hirsch\Desktop\StauRob\include\mapping\Localizer.h"

class Mapper {
    std::unordered_map<Pair, std::string> map;
    int mapId;
    Localizer* localizer = nullptr;
    //TODO event queue

public:
    Mapper(Localizer* loc) : localizer(loc) {}
    void setCell(Pair pair, const std::string& state);

    void save(const std::string& filename);
    void load(const std::string& filename);

    std::vector<Pair> calculatePath(const Pair& start, const Pair& end);
    void update(); //TODO, ließt event-queue aus
};