#include "JSONParser.h"
#include <fstream>
#include "Methods.h"
#include "MethodFactory.h"

void JSONParser::loadJSON(const string &filePath) {
    ifstream file(filePath);
    if (!file) {
        cerr << "Error: Cannot open file " << filePath << endl;
        exit(1);
    }

    try {
        file >> config;
    } catch (const json::parse_error &e) {
        cerr << "Error: JSON parsing error: " << e.what() << endl;
        exit(1);
    }
}

json JSONParser::getConfigValue(const vector<string> &keys) const {
    json current = config;
    vector<string> key_path;

    for (const auto &key: keys) {
        if (!current.contains(key))
            throw runtime_error("Key '" + key + "' does not exist! Key path: " + makeKeyPath(key_path));

        current = current[key];
        key_path.push_back(key);
    }

    return current;
}

vector<json> JSONParser::filterExistingMethodsConfig(const vector<string>& methodsName) const {
    json methods_config = getConfigValue({METHODS});
    vector<json> filtered_methods_config;

    for (const auto &methodName: methodsName)
        if (methods_config.contains(methodName)) filtered_methods_config.push_back(methods_config[methodName]);
        else cerr << "Warning: Method '" << methodName << "' not found!" << endl;

    return filtered_methods_config;
}

vector<json> JSONParser::getSelectedBoxPlotMethodsConfig() const {
    vector<string> keys = {PLOT_CONFIG, BOX_PLOT, BOX_PLOT_METHODS};
    json box_plot_methods = getConfigValue(keys);

    if (!box_plot_methods.is_array())
        throw runtime_error("Error: '" + keys.back() + "' is not an array! Key path: " + makeKeyPath(keys));

    for (const auto &method: box_plot_methods)
        if (!method.is_string())
            throw runtime_error("Error: '" + keys.back() + "' contains non-string values! Key path: " + makeKeyPath(keys));

    return filterExistingMethodsConfig(box_plot_methods.get<vector<string>>());
}

vector<json> JSONParser::getSelectedSinglePlotMethodsConfig() const {
    vector<string> keys = {PLOT_CONFIG, SINGLE_PLOT};
    json single_plot_methods = getConfigValue(keys);

    if (!single_plot_methods.is_array())
        throw runtime_error("Error: '" + keys.back() + "' is not an array! Key path: " + makeKeyPath(keys));

    for (const auto &method: single_plot_methods)
        if (!method.is_string())
            throw runtime_error("Error: '" + keys.back() + "' contains non-string values! Key path: " + makeKeyPath(keys));

    return filterExistingMethodsConfig(single_plot_methods.get<vector<string>>());
}

int JSONParser::getBoxPlotIterations() const {
    vector<string> keys = {PLOT_CONFIG, BOX_PLOT, BOX_PLOT_ITERATIONS};
    json iterations = getConfigValue(keys);

    if (!iterations.is_number())
        throw runtime_error("Error: '" + keys.back() + "' is not a number! Key path: " + makeKeyPath(keys));

    if (iterations.get<int>() <= 0)
        throw runtime_error("Error: '" + keys.back() + "' is not a positive number! Key path: " + makeKeyPath(keys));

    return iterations.get<int>();
}

string JSONParser::makeKeyPath(const vector<string> &keys) const {
    string key_path = ".";
    for (const auto &key: keys) key_path += key + ".";
    return key_path;
}
