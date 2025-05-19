#include <fstream>
#include <sstream>
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\mapping\Mapper.h"

void Mapper::setCell(Pair pair, const std::string& state) {
    map[pair] = state;
}

void Mapper::update() {
    if (!localizer) return;

    Pair pos = localizer->getPosition();
    setCell(pos, "visited");
}


// Simplified saving function
void Mapper::save(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) return;

    file << "MapID " << mapId << "\n";
    for (const auto& [key, value] : map) {
        file << key.x << " " << key.y << " " << value << "\n";
    }
}

// Simplified loading function
void Mapper::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return;

    std::string line;
    map.clear();

    std::getline(file, line);
    std::istringstream idLine(line);
    std::string label;
    idLine >> label >> mapId;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        Pair p;
        std::string state;
        ss >> p.x >> p.y >> state;
        map[p] = state;
    }
}