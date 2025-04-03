#include "MethodFactory.h"

unique_ptr<Method> MethodFactory::createMethod(const json &methodConfig, Problem &problem) const {
    string type = parseString(methodConfig, "type");
    string mutation_type = parseString(methodConfig, "mut");

    unique_ptr<Mutation> mutation = createMutation(mutation_type, problem);

    if (type == "tabu")
        return createTabuMethod(methodConfig, problem, std::move(mutation));
    else if (type == "annealing")
        return createSimulatedAnnealingMethod(methodConfig, problem, std::move(mutation));
    else if (type == "hybrid")
        return createHybridTabuSAMethod(methodConfig, problem, std::move(mutation));

    return nullptr;
}

vector<unique_ptr<Method>>
MethodFactory::createMethodManyTimes(const json &methodConfig, Problem &problem, int count) const {
    vector<unique_ptr<Method>> methods;
    methods.reserve(count);

    for (int i = 0; i < count; ++i) {
        auto method = createMethod(methodConfig, problem);
        if (method) methods.push_back(std::move(method));
        else {
            cerr << "Error: Failed to create method(s) of type '" << parseString(methodConfig, "type")
                 << "'! No such method type exists!" << endl;
            return {};
        }
    }

    return methods;
}

string MethodFactory::parseString(const json &methodConfig, const string &key) const {
    return methodConfig.at(key).get<string>();
}

int MethodFactory::parseInt(const json &methodConfig, const string &key) const {
    return methodConfig.at(key).get<int>();
}

double MethodFactory::parseDouble(const json &methodConfig, const string &key) const {
    return methodConfig.at(key).get<double>();
}

unique_ptr<Mutation> MethodFactory::createMutation(const string &mutationType, Problem &problem) const {
    if (mutationType == "swap") return make_unique<SingleSwapMutation>(problem.getGenotypeSize());
    else return make_unique<InversionMutation>(problem.getGenotypeSize());
}

unique_ptr<Method> MethodFactory::createTabuMethod(const json &methodConfig,
                                                   Problem &problem,
                                                   unique_ptr<Mutation> mutation) const {
    int iterations = parseInt(methodConfig, "iter");
    int tabuSize = parseInt(methodConfig, "tabuSize");
    int neighborhoodSize = parseInt(methodConfig, "neighborhoodSize");

    return make_unique<TabuSearchMethod>(problem,
                                         iterations,
                                         std::move(mutation),
                                         tabuSize,
                                         neighborhoodSize);
}

unique_ptr<Method> MethodFactory::createSimulatedAnnealingMethod(const json &methodConfig,
                                                                 Problem &problem,
                                                                 unique_ptr<Mutation> mutation) const {
    int iterations = parseInt(methodConfig, "iter");
    int neighborhoodSize = parseInt(methodConfig, "neighborhoodSize");

    double initTemp = parseDouble(methodConfig, "initTemp");
    double finalTemp = parseDouble(methodConfig, "finalTemp");
    double coolingRatio = parseDouble(methodConfig, "coolingRatio");
    string coolSchemeName = parseString(methodConfig, "coolingScheme");

    auto coolingScheme = (coolSchemeName == "linear") ? linear : geometric;

    return make_unique<SimulatedAnnealingMethod>(problem,
                                                 iterations,
                                                 std::move(mutation),
                                                 neighborhoodSize,
                                                 initTemp,
                                                 finalTemp,
                                                 coolingRatio,
                                                 coolingScheme,
                                                 coolSchemeName);
}

unique_ptr<Method> MethodFactory::createHybridTabuSAMethod(const json &methodConfig,
                                                           Problem &problem,
                                                           unique_ptr<Mutation> mutation) const {
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
    string heatSchemeName = parseString(methodConfig, "heatingScheme");

    auto coolingScheme = (coolSchemeName == "linear") ? linear : geometric;
    auto heatingScheme = (heatSchemeName == "linear") ? linear : geometric;

    return make_unique<HybridTabuSAMethod>(problem,
                                           iterations,
                                           std::move(mutation),
                                           iterHeating,
                                           tabuSize,
                                           neighborhoodSize,
                                           initTemp,
                                           finalTemp,
                                           maxHeatingTemp,
                                           coolingRatio,
                                           coolingScheme,
                                           coolSchemeName,
                                           heatingRatio,
                                           heatingScheme,
                                           heatSchemeName);
}
