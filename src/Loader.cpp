#include <fstream>
#include <sstream>
#include "Loader.h"
#include "iostream"

optional<Loader::Data> Loader::loadProblemFromFile(const path &filepath) {
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
        else if (key == "DEPOT_SECTION") parseDepot(file, data);
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

void Loader::parseDepot(ifstream &file, Data &data) {
    file >> data.depot;
}

bool Loader::validateData(const Data &data) {
    return data.cities.size() == data.dimension && data.demands.size() == data.dimension;
}

void Loader::Data::print() const {
    cout << "Name: " << name << endl;
    cout << "Comment: " << comment << endl;
    cout << "Type: " << type << endl;
    cout << "Dimension: " << dimension << endl;
    cout << "Edge weight type: " << edge_weight_type << endl;
    cout << "Capacity: " << capacity << endl;
    cout << "Depot: " << depot << endl;

    cout << "Demands:" << endl;
    for (auto &demand: demands)
        cout << "City: " << demand.city_id << " Demand: " << demand.value << endl;
}
