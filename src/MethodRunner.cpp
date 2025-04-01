#include "MethodRunner.h"

void MethodRunner::runMethods() {
    if (!config.contains("config")) {
        cerr << "Error: 'config' section is missing!" << endl;
        return;
    }

    json configData = config["config"];

    if (configData.contains("boxPlot")) runBoxPlotMethods(configData["boxPlot"]);
    if (configData.contains("singlePlot")) runSinglePlotMethods(configData);
}

vector<Method *> MethodRunner::getSelectedMethods(const json& config, const string& key) {
    vector<string> methodNames = extractMethodsFromJson(config, key);
    vector<Method *> selectedMethods;
    selectedMethods.reserve(methodNames.size());

    for (const string& methodName : methodNames)
        if (methods.find(methodName) != methods.end()) selectedMethods.push_back(methods[methodName].get());
        else cerr << "Warning: Method '" << methodName << "' not found!" << endl;

    return selectedMethods;
}

void MethodRunner::runBoxPlotMethods(const json& boxPlotConfig) {
    vector<Method *> selectedMethods = getSelectedMethods(boxPlotConfig, "methods");
    int iterations = extractIterationsFromJson(boxPlotConfig, "iter");

    if (selectedMethods.empty() || iterations == -1) return;

    Method::runEachMethodManyTimesAndSave(problem, selectedMethods, iterations);
}

void MethodRunner::runSinglePlotMethods(const json& singlePlotConfig) {
    vector<Method *> selectedMethods = getSelectedMethods(singlePlotConfig, "singlePlot");

    if (selectedMethods.empty()) return;

    Method::runEachMethodOnceAndSave(selectedMethods);
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