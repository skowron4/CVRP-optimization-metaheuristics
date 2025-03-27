#include <fstream>
#include <sstream>
#include "Loader.h"
#include "iostream"

optional<Loader::Data> Loader::loadProblemFromFile(const string &filepath) {
    ifstream file(filepath);

    if (!file.is_open()) {
        cerr << "Error: Cannot open file" << filepath << "\n";
        return nullopt;
    }

    Data data;
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        iss >> key;

        if (key == "NAME") parseKey(iss, data.name);
        else if (key == "DIMENSION") parseKey(iss, data.dimension);
        else if (key == "CAPACITY") parseKey(iss, data.capacity);
        else if (key == "NODE_COORD_SECTION") parseCitySection(file, data);
        else if (key == "DEMAND_SECTION") parseDemandSection(file, data);
        else if (key == "DEPOT_SECTION") parseDepot(line, data);
        else if (key == "EOF") break;
    }

    if (!validateData(data)) {
        cerr << "Error: numbers of cities doesn't fit\n";
        return nullopt;
    }

    return make_optional<Data>(data);
}

void Loader::parseKey(istringstream &iss, int &value) {
    string temp;
    iss >> temp >> value;
}

void Loader::parseKey(istringstream &iss, string &value) {
    string temp;
    iss >> temp >> value;
}

void Loader::parseCitySection(ifstream &file, Data &data) {
    data.cities.reserve(data.dimension);
    for (int i = 0; i < data.dimension; ++i) {
        City city{};
        file >> city.id >> city.x >> city.y;
        data.cities.push_back(city);
    }
}

void Loader::parseDemandSection(ifstream &file, Data &data) {
    data.demands.reserve(data.dimension);
    for (int i = 0; i < data.dimension; ++i) {
        Demand demand{};
        file >> demand.city_id >> demand.value;
        data.demands.push_back(demand);
    }
}

void Loader::parseDepot(const string &line, Data &data) {
    istringstream iss(line);
    iss >> data.depot;
}

bool Loader::validateData(const Data &data) {
    return data.cities.size() == data.dimension && data.demands.size() == data.dimension;
}
