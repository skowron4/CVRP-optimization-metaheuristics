#ifndef LOADER_H
#define LOADER_H

#include "Problem.h"
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

    optional<Problem> loadProblemFromFile(const string &filepath);
}

#endif //LOADER_H
