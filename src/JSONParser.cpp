#include "JSONParser.h"
#include <fstream>
#include "Methods.h"
#include "unordered_map"
#include "MethodFactory.h"

json JSONParser::loadJSON(const string& filePath) {
    ifstream file(filePath);
    if (!file) {
        cerr << "Error: Cannot open file " << filePath << endl;
        exit(1);
    }

    json jsonData;
    file >> jsonData;
    return jsonData;
}

unordered_map<string, unique_ptr<Method>> JSONParser::parseJSONAndCreateMethods(const json& config,
                                                                    Problem& problem,
                                                                    SingleSwapMutation& singleSwapMutation,
                                                                    InversionMutation& inversionMutation,
                                                                    double (*linear)(double, double),
                                                                    double (*geometric)(double, double),
                                                                    mt19937& randomEngine) {
    unordered_map<string, unique_ptr<Method>> methods;
    for (auto& [key, methodConfig] : config.items()) {
        try {
            unique_ptr<Method> method = MethodFactory::createMethodFromJson(methodConfig,
                                                             problem,
                                                             singleSwapMutation,
                                                             inversionMutation,
                                                             linear,
                                                             geometric,
                                                             randomEngine);
            if (method) methods.emplace(key, std::move(method));
            else cout << "Unknown method name: " << methodConfig.at("name").get<string>() << endl;

        } catch (const json::exception& e) {
            cerr << "Error creating method " << key << ": " << e.what() << endl;
        }
    }
    return std::move(methods);
}