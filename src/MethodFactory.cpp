//
// Created by User on 26.03.2025.
//

#include "MethodFactory.h"

std::string MethodFactory::parseString(const json& methodConfig, const std::string& key) {
    return methodConfig.at(key).get<std::string>();
}

int MethodFactory::parseInt(const json& methodConfig, const std::string& key) {
    return methodConfig.at(key).get<int>();
}

double MethodFactory::parseDouble(const json& methodConfig, const std::string& key) {
    return methodConfig.at(key).get<double>();
}

Mutation* MethodFactory::selectMutation(const json& methodConfig,
                         SingleSwapMutation& singleSwapMutation,
                         InversionMutation& inversionMutation) {
    std::string mutationType = parseString(methodConfig, "mut");
    if (mutationType == "swap") {
        return &singleSwapMutation;
    } else {
        return &inversionMutation;
    }
}

std::unique_ptr<Method> MethodFactory::createTabuMethod(const json& methodConfig,
                                         Problem& problem,
                                         Mutation* mutation,
                                         std::mt19937& randomEngine) {
    int iterations = parseInt(methodConfig, "iter");
    int tabuSize = parseInt(methodConfig, "tabuSize");
    int neighborhoodSize = parseInt(methodConfig, "neighborhoodSize");

    return std::make_unique<TabuSearchMethod>(problem,
                                              iterations,
                                              *mutation,
                                              tabuSize,
                                              neighborhoodSize,
                                              randomEngine);
}

std::unique_ptr<Method> MethodFactory::createSimulatedAnnealingMethod(const json& methodConfig,
                                                       Problem& problem,
                                                       Mutation* mutation,
                                                       std::mt19937& randomEngine,
                                                       double (*linear)(double, double),
                                                       double (*geometric)(double, double)) {
    int iterations = parseInt(methodConfig, "iter");
    int neighborhoodSize = parseInt(methodConfig, "neighborhoodSize");

    double initTemp = parseDouble(methodConfig, "initTemp");
    double finalTemp = parseDouble(methodConfig, "finalTemp");
    double coolingRatio = parseDouble(methodConfig, "coolingRatio");
    std::string coolSchemeName = parseString(methodConfig, "coolingScheme");

    auto coolingScheme = (coolSchemeName == "linear") ? linear : geometric;

    return std::make_unique<SimulatedAnnealingMethod>(problem,
                                                      iterations,
                                                      *mutation,
                                                      neighborhoodSize,
                                                      initTemp,
                                                      finalTemp,
                                                      coolingRatio,
                                                      coolingScheme,
                                                      coolSchemeName,
                                                      randomEngine);
}

std::unique_ptr<Method> MethodFactory::createHybridTabuSAMethod(const json& methodConfig,
                                                 Problem& problem,
                                                 Mutation* mutation,
                                                 std::mt19937& randomEngine,
                                                 double (*linear)(double, double),
                                                 double (*geometric)(double, double)) {
    int iterations = parseInt(methodConfig, "iter");
    int neighborhoodSize = parseInt(methodConfig, "neighborhoodSize");
    int iterHeating = parseInt(methodConfig, "iterHeating");
    int tabuSize = parseInt(methodConfig, "tabuSize");

    double initTemp = parseDouble(methodConfig, "initTemp");
    double finalTemp = parseDouble(methodConfig, "finalTemp");
    double maxHeatingTemp = parseDouble(methodConfig, "maxHeatingTemp");
    double coolingRatio = parseDouble(methodConfig, "coolingRatio");
    double heatingRatio = parseDouble(methodConfig, "heatingRatio");

    std::string coolSchemeName = parseString(methodConfig, "coolingScheme");
    std::string heatSchemeName = parseString(methodConfig, "coolingScheme"); // Assuming same key for heatScheme

    auto coolingScheme = (coolSchemeName == "linear") ? linear : geometric;
    auto heatingScheme = (heatSchemeName == "linear") ? linear : geometric;

    return std::make_unique<HybridTabuSAMethod>(
            problem, iterations, iterHeating, *mutation, tabuSize, neighborhoodSize,
            initTemp, finalTemp, maxHeatingTemp, coolingScheme, coolingRatio,
            heatingRatio, heatingScheme, coolSchemeName, heatSchemeName, randomEngine
    );
}