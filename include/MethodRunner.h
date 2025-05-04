#ifndef CVRP_OPTIMIZATION_METAHEURISTICS_METHODRUNNER_H
#define CVRP_OPTIMIZATION_METAHEURISTICS_METHODRUNNER_H

#include "Problem.h"
#include <json.hpp>
#include <filesystem>
#include "MethodFactory.h"
#include "JSONParser.h"

using json = nlohmann::json;
using namespace std;
using namespace std::filesystem;

class MethodRunner {
public:
    MethodRunner(Problem &problem, JSONParser &jsonParser) : problem(problem), json_parser(jsonParser) {}

    void runConfig(const path &destDir) const;

private:
    Problem &problem;
    JSONParser &json_parser;
    MethodFactory method_factory;

    void runBoxPlotMethods(const path &outDir) const;

    void runSinglePlotMethods(const path &outDir) const;
};

#endif //CVRP_OPTIMIZATION_METAHEURISTICS_METHODRUNNER_H
