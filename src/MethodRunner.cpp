#include "MethodRunner.h"

void MethodRunner::runConfig() {
    runBoxPlotMethods();
    runSinglePlotMethods();
}

void MethodRunner::runBoxPlotMethods() {
    vector<json> methods_config;
    int iterations;

    try {
        methods_config = json_parser.getSelectedBoxPlotMethodsConfig();
        iterations = json_parser.getBoxPlotIterations();
    } catch (const runtime_error &e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }

    if (methods_config.empty()) {
        cerr << "No methods selected for box plot!" << endl;
        return;
    }

    vector<vector<unique_ptr<Method>>> methods(methods_config.size());

    for (int i = 0; i < methods_config.size(); ++i) {
        auto created_methods = method_factory.createMethodManyTimes(methods_config[i], problem, iterations);
        if (!created_methods.empty()) methods[i] = std::move(created_methods);
        else {
            cerr << "Abandoning box plot data generation!" << endl;
            return;
        }
    }

    Method::runEachMethodAndSaveBoxPlot(problem, methods);
}

void MethodRunner::runSinglePlotMethods() {
    vector<json> methods_config;

    try {
        methods_config = json_parser.getSelectedSinglePlotMethodsConfig();
    } catch (const runtime_error &e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }

    if (methods_config.empty()) {
        cerr << "No methods selected for single plot!" << endl;
        return;
    }

    vector<unique_ptr<Method>> methods;
    methods.reserve(methods_config.size());

    for (const auto &method_config: methods_config) {
        auto method = method_factory.createMethodManyTimes(method_config, problem, 1);
        if (!method.empty()) methods.push_back(std::move(method[0]));
        else {
            cerr << "Abandoning single plot data generation!" << endl;
            return;
        }
    }

    Method::runEachMethodAndSaveSinglePlots(methods);
}
