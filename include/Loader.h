#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <optional>
#include <string>

using namespace std;

namespace Loader {
    struct City {
        int id, x, y;
    };

    struct Demand {
        int city_id, value;
    };

    struct Data {
        string filename, name, comment, type, edge_weight_type;
        vector<City> cities;
        vector<Demand> demands;
        int dimension, capacity, depot;
    };

    /* I thought about it a lot and came to the conclusion that we should return only data struct
     * Why? Because solid and circular dependencies.
     * First: we should read only the data and return it
     * Second: We should pass it as a parameter to the problem constructor, for example in main
     */

    optional<Loader::Data> loadProblemFromFile(const string &filepath);

    void parseKey(std::istringstream &iss, int &value);

    void parseCity(const std::string &line, Data &data);

    void parseDemand(const std::string &line, Data &data);

    void parseDepot(const std::string &line, Data &data);

    bool validateData(const Data &data) {
        return data.cities.size() == data.dimension || data.demands.size() == data.dimension;
    }
}

#endif //LOADER_H
