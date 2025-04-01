#ifndef CVRP_OPTIMIZATION_METAHEURISTICS_JSONPARSER_H
#define CVRP_OPTIMIZATION_METAHEURISTICS_JSONPARSER_H

#include <json.hpp>
#include "iostream"
#include "Problem.h"
#include "Mutations.h"
#include "Methods.h"

using json = nlohmann::json;
using namespace std;

class JSONParser {
public:
    static json loadJSON(const string& filePath);
    static unordered_map<string, unique_ptr<Method>> parseJSONAndCreateMethods(const json& config,
                                                                               Problem& problem,
                                                                               SingleSwapMutation& singleSwapMutation,
                                                                               InversionMutation& inversionMutation,
                                                                               double (*linear)(double, double),
                                                                               double (*geometric)(double, double),
                                                                               mt19937& randomEngine);
};

#endif //CVRP_OPTIMIZATION_METAHEURISTICS_JSONPARSER_H
