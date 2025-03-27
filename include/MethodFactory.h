//
// Created by User on 26.03.2025.
//

#ifndef CVRP_OPTIMIZATION_METAHEURISTICS_METHODFACTORY_H
#define CVRP_OPTIMIZATION_METAHEURISTICS_METHODFACTORY_H

#include "iostream"
#include "Methods.h"
#include <json.hpp>

using json = nlohmann::json;

class MethodFactory {
public:
    static std::unique_ptr<Method> createMethodFromJson(
            const json& methodConfig,
            Problem& problem,
            SingleSwapMutation& singleSwapMutation,
            InversionMutation& inversionMutation,
            double (*linear)(double, double),
            double (*geometric)(double, double),
            std::mt19937& randomEngine) {
        std::string name = parseString(methodConfig, "name");

        Mutation* mutation = selectMutation(methodConfig, singleSwapMutation, inversionMutation);

        if (name == "tabu")
            return createTabuMethod(methodConfig, problem, mutation, randomEngine);
        else if (name == "annealing")
            return createSimulatedAnnealingMethod(methodConfig, problem, mutation, randomEngine, linear, geometric);
        else if (name == "hybrid")
            return createHybridTabuSAMethod(methodConfig, problem, mutation, randomEngine, linear, geometric);


        return nullptr;
    }

private:
    static std::string parseString(const json& methodConfig, const std::string& key);

    static int parseInt(const json& methodConfig, const std::string& key);

    static double parseDouble(const json& methodConfig, const std::string& key);

    static Mutation* selectMutation(const json& methodConfig,
                             SingleSwapMutation& singleSwapMutation,
                             InversionMutation& inversionMutation);

    static std::unique_ptr<Method> createTabuMethod(const json& methodConfig,
                                             Problem& problem,
                                             Mutation* mutation,
                                             std::mt19937& randomEngine);

    static std::unique_ptr<Method> createSimulatedAnnealingMethod(const json& methodConfig,
                                                           Problem& problem,
                                                           Mutation* mutation,
                                                           std::mt19937& randomEngine,
                                                           double (*linear)(double, double),
                                                           double (*geometric)(double, double));

    static std::unique_ptr<Method> createHybridTabuSAMethod(const json& methodConfig,
                                                     Problem& problem,
                                                     Mutation* mutation,
                                                     std::mt19937& randomEngine,
                                                     double (*linear)(double, double),
                                                     double (*geometric)(double, double));


};


#endif //CVRP_OPTIMIZATION_METAHEURISTICS_METHODFACTORY_H
