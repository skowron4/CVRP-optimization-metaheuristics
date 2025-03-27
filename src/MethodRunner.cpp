//
// Created by User on 26.03.2025.
//

#include "MethodRunner.h"
void MethodRunner::runMethods() {
    if (!config.contains("config")) {
        cerr << "Error: 'config' section is missing!" << endl;
        return;
    }

    json configData = config["config"];

    if (configData.contains("boxPlot"))
        runBoxPlotMethods(configData["boxPlot"]);

    if (configData.contains("singlePlot"))
        runSinglePlotMethods(configData);
}

void MethodRunner::runBoxPlotMethods(const json& boxPlotConfig) {

    vector<string> methodNames = extractMethodsFromJson(boxPlotConfig, "methods");
    int iterations = extractIterationsFromJson(boxPlotConfig, "iter");

    if (methodNames.empty() || iterations == -1) return;

    Method::runEachMethodManyTimesAndSave(problem, methods, iterations);
}

void MethodRunner::runSinglePlotMethods(const json& singlePlotConfig) {

    vector<string> methodNames = extractMethodsFromJson(singlePlotConfig, "singlePlot");
    if (methodNames.empty()) return;

    for (const string& methodName : methodNames) {
        if (methods.find(methodName) != methods.end())
            methods[methodName]->runAndSave();
        else
            cerr << "Warning: Method '" << methodName << "' not found!" << endl;
    }
}

vector<string> MethodRunner::extractMethodsFromJson(const json& plotConfig, const string& key) {
    if (!plotConfig.contains(key) || !plotConfig[key].is_array()) {
        cerr << "Error: Key '" << key << "' does not exist or is not an array!" << endl;
        return {};
    }
    return plotConfig[key].get<vector<string>>();
}

int MethodRunner::extractIterationsFromJson(const json& plotConfig, const string& key) {
    if (!plotConfig.contains(key) || !plotConfig[key].is_number()) {
        cerr << "Error: '" << key << "' does not exist or is not a number!" << endl;
        return -1;
    }
    return plotConfig[key].get<int>();
}