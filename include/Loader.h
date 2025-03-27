#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <optional>
#include <string>

using namespace std;

class Loader {
public:
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

    optional<Data> loadProblemFromFile(const string &filepath);

private:
    void parseKey(istringstream &iss, int &value);

    void parseKey(istringstream &iss, string &value);

    void parseCitySection(ifstream &file, Data &data);

    void parseDemandSection(ifstream &file, Data &data);

    void parseDepot(const string &line, Data &data);

    bool validateData(const Data &data);
};

#endif //LOADER_H
