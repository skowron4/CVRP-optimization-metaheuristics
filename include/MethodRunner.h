#ifndef CVRP_OPTIMIZATION_METAHEURISTICS_METHODRUNNER_H
#define CVRP_OPTIMIZATION_METAHEURISTICS_METHODRUNNER_H

#include "Problem.h"
#include <json.hpp>
#include "Methods.h"

using json = nlohmann::json;

class MethodRunner {
public:
    MethodRunner(Problem& problem,
                 const json& config,
                 std::unordered_map<std::string, std::unique_ptr<Method>>& methods) :
                 problem(problem),
                 config(config),
                 methods(methods) {}

    void runMethods();

private:
    Problem &problem;
    const json &config;
    std::unordered_map<std::string, std::unique_ptr<Method>> &methods;

    void runBoxPlotMethods(const json &boxPlotConfig);

    void runSinglePlotMethods(const json &singlePlotConfig);

    std::vector<std::string> extractMethodsFromJson(const json &config, const std::string &key);

    int extractIterationsFromJson(const json &config, const std::string &key);

    vector<Method *> getSelectedMethods(const json &config, const string &key);
};
#endif //CVRP_OPTIMIZATION_METAHEURISTICS_METHODRUNNER_H
