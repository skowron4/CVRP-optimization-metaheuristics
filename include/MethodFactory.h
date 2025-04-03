#ifndef CVRP_OPTIMIZATION_METAHEURISTICS_METHODFACTORY_H
#define CVRP_OPTIMIZATION_METAHEURISTICS_METHODFACTORY_H

#include "iostream"
#include "Methods.h"
#include <json.hpp>
#include <utility>

using json = nlohmann::json;
using namespace std;

class MethodFactory {
public:
    vector<unique_ptr<Method>> createMethodManyTimes(const json &methodConfig, Problem &problem, int count) const;

private:
    inline static auto geometric = [](double temp, double ratio) { return temp * ratio; };
    inline static auto linear = [](double temp, double ratio) { return temp + ratio; };

    unique_ptr<Method> createMethod(const json &methodConfig, Problem &problem) const;

    string parseString(const json &methodConfig, const string &key) const;

    int parseInt(const json &methodConfig, const string &key) const;

    double parseDouble(const json &methodConfig, const string &key) const;

    unique_ptr<Mutation> createMutation(const string &mutationType, Problem &problem) const;

    unique_ptr<Method> createTabuMethod(const json &methodConfig,
                                        Problem &problem,
                                        unique_ptr<Mutation> mutation) const;

    unique_ptr<Method> createSimulatedAnnealingMethod(const json &methodConfig,
                                                      Problem &problem,
                                                      unique_ptr<Mutation> mutation) const;

    unique_ptr<Method> createHybridTabuSAMethod(const json &methodConfig,
                                                Problem &problem,
                                                unique_ptr<Mutation> mutation) const;
};

#endif //CVRP_OPTIMIZATION_METAHEURISTICS_METHODFACTORY_H
