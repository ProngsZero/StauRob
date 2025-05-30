#include "mapping/Mapper.h"

Mapper::Mapper(Localizer* loc, char* state, CRITICAL_SECTION* mutex, CRITICAL_SECTION* updateMutex) 
{
    this->localizer = loc;
    this->stateMutex = mutex;
    this->stateString = state;
    this->updateMutex = updateMutex;
}

void Mapper::setCell(Pair pair, const std::string& state) 
{
    map[pair] = state;
}

void Mapper::update() 
{
    Pair pos = localizer->getPosition();
    setCell(pos, "visited");
}

void Mapper::save(const std::string& filename) 
{
    std::ofstream file(filename);
    if (!file) return;

    file << "MapID " << mapId << "\n";
    for (const auto& [key, value] : map) 
    {
        file << key.x << " " << key.y << " " << value << "\n";
    }
}

void Mapper::load(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file) return;

    std::string line;
    map.clear();

    std::getline(file, line);
    std::istringstream idLine(line);
    std::string label;
    idLine >> label >> mapId;

    while (std::getline(file, line)) 
    {
        std::istringstream ss(line);
        Pair p;
        std::string state;
        ss >> p.x >> p.y >> state;
        map[p] = state;
    }
}