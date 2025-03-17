#include <fstream>
#include <sstream>
#include "Loader.h"
#include "iostream"

using namespace std;

optional<Loader::Data> Loader::loadProblemFromFile(const string &filepath) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file" << filepath << "\n";
        return nullopt;
    }

    Data data;
    std::string line;
    bool inCitySection{false}, inDemandSection{false}, inDepotSection{false};

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "DIMENSION") {
            parseKey(iss, data.dimension);
        } else if (key == "CAPACITY") {
            parseKey(iss, data.capacity);
        } else if (key == "NODE_COORD_SECTION") {
            parseCity(line, data);
            continue;
        } else if (key == "DEMAND_SECTION") {
            parseDemand(line, data);
            continue;
        } else if (key == "DEPOT_SECTION") {
            parseDepot(line, data);
            continue;
        } else if (key == "EOF") {
            break;
        }
    }

    if (validateData(data)) {
        std::cerr << "Error: numbers of cities doesn't fit\n";
        return nullopt;
    }

    return make_optional<Data>(data);
}

void Loader::parseKey(std::istringstream &iss, int &value) {
    std::string temp;
    iss >> temp >> value;
}

void Loader::parseCity(const std::string &line, Data &data) {
    City city{};
    std::istringstream iss(line);
    iss >> city.id >> city.x >> city.y;
    data.cities.push_back(city);
}

void Loader::parseDemand(const std::string &line, Data &data) {
    Demand demand{};
    std::istringstream iss(line);
    iss >> demand.city_id >> demand.value;
    data.demands.push_back(demand);
}

void Loader::parseDepot(const std::string &line, Data &data) {
    std::istringstream iss(line);
    iss >> data.depot;
}