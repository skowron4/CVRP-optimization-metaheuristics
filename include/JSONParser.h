#ifndef CVRP_OPTIMIZATION_METAHEURISTICS_JSONPARSER_H
#define CVRP_OPTIMIZATION_METAHEURISTICS_JSONPARSER_H

#include <json.hpp>
#include "iostream"
#include <filesystem>
#include "Problem.h"
#include "Mutations.h"
#include "Methods.h"

using json = nlohmann::json;
using namespace std;
using namespace std::filesystem;

class JSONParser {
public:
    explicit JSONParser(const path &configurationFilePath) { loadJSON(configurationFilePath); }

    vector<json> getSelectedBoxPlotMethodsConfig() const;

    vector<json> getSelectedSinglePlotMethodsConfig() const;

    int getBoxPlotIterations() const;

private:
    // JSON keys
    inline static const string METHODS = "methods";
    inline static const string PLOT_CONFIG = "plotConfig";
    inline static const string BOX_PLOT = "boxPlot";
    inline static const string BOX_PLOT_METHODS = "methods";
    inline static const string BOX_PLOT_ITERATIONS = "iter";
    inline static const string SINGLE_PLOT = "singlePlot";

    json config;

    void loadJSON(const path &filePath);

    json getConfigValue(const vector<string> &keys) const;

    vector<json> filterExistingMethodsConfig(const vector<string>& methodsName) const;

    string makeKeyPath(const vector<string> &keys) const;
};

#endif //CVRP_OPTIMIZATION_METAHEURISTICS_JSONPARSER_H
