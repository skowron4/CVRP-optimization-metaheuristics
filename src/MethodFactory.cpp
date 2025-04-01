#include "MethodFactory.h"

string MethodFactory::parseString(const json& methodConfig, const string& key) {
    return methodConfig.at(key).get<string>();
}

int MethodFactory::parseInt(const json& methodConfig, const string& key) {
    return methodConfig.at(key).get<int>();
}

double MethodFactory::parseDouble(const json& methodConfig, const string& key) {
    return methodConfig.at(key).get<double>();
}

Mutation* MethodFactory::selectMutation(const json& methodConfig,
                         SingleSwapMutation& singleSwapMutation,
                         InversionMutation& inversionMutation) {
    string mutationType = parseString(methodConfig, "mut");
    if (mutationType == "swap") return &singleSwapMutation;
    else return &inversionMutation;
}

unique_ptr<Method> MethodFactory::createTabuMethod(const json& methodConfig,
                                         Problem& problem,
                                         Mutation* mutation,
                                         mt19937& randomEngine) {
    int iterations = parseInt(methodConfig, "iter");
    int tabuSize = parseInt(methodConfig, "tabuSize");
    int neighborhoodSize = parseInt(methodConfig, "neighborhoodSize");

    return make_unique<TabuSearchMethod>(problem,
                                              iterations,
                                              *mutation,
                                              tabuSize,
                                              neighborhoodSize,
                                              randomEngine);
}

unique_ptr<Method> MethodFactory::createSimulatedAnnealingMethod(const json& methodConfig,
                                                       Problem& problem,
                                                       Mutation* mutation,
                                                       mt19937& randomEngine,
                                                       double (*linear)(double, double),
                                                       double (*geometric)(double, double)) {
    int iterations = parseInt(methodConfig, "iter");
    int neighborhoodSize = parseInt(methodConfig, "neighborhoodSize");

    double initTemp = parseDouble(methodConfig, "initTemp");
    double finalTemp = parseDouble(methodConfig, "finalTemp");
    double coolingRatio = parseDouble(methodConfig, "coolingRatio");
    string coolSchemeName = parseString(methodConfig, "coolingScheme");

    auto coolingScheme = (coolSchemeName == "linear") ? linear : geometric;

    return make_unique<SimulatedAnnealingMethod>(problem,
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

unique_ptr<Method> MethodFactory::createHybridTabuSAMethod(const json& methodConfig,
                                                 Problem& problem,
                                                 Mutation* mutation,
                                                 mt19937& randomEngine,
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

    string coolSchemeName = parseString(methodConfig, "coolingScheme");
    string heatSchemeName = parseString(methodConfig, "coolingScheme"); // Assuming same key for heatScheme

    auto coolingScheme = (coolSchemeName == "linear") ? linear : geometric;
    auto heatingScheme = (heatSchemeName == "linear") ? linear : geometric;

    return make_unique<HybridTabuSAMethod>(
            problem, iterations, iterHeating, *mutation, tabuSize, neighborhoodSize,
            initTemp, finalTemp, maxHeatingTemp, coolingScheme, coolingRatio,
            heatingRatio, heatingScheme, coolSchemeName, heatSchemeName, randomEngine
    );
}