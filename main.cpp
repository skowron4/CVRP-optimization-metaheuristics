#include <iostream>
#include <json.hpp>
#include <fstream>
#include "Methods.h"
#include "unordered_map"
#include "JSONParser.h"
#include "MethodRunner.h"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <config_file_path>" << std::endl;
        return 0;
    }

    string configFilePath = argv[1];
    string dataFilePath = argv[2];

    Loader loader;
    auto data = loader.loadProblemFromFile(dataFilePath);
    if (!data.has_value()) {
        std::cerr << "Error while loading data from file" << std::endl;
        return 1;
    }

    Problem problem(data.value());

    std::mt19937 randomEngine(std::chrono::system_clock::now().time_since_epoch().count());
    SingleSwapMutation singleSwapMutation(randomEngine);
    InversionMutation inversionMutation(randomEngine);

    auto geometric = [](double temp, double ratio) {return temp * ratio;};
    auto linear = [](double temp, double ratio) {return temp + ratio;};

    json config = JSONParser::loadJSON(configFilePath);

    unordered_map<string, unique_ptr<Method>> methods =
            JSONParser::parseJSONAndCreateMethods(config.at("methods"),
                                                  problem,
                                                  singleSwapMutation,
                                                  inversionMutation,
                                                  geometric,
                                                  linear,
                                                  randomEngine);

    MethodRunner runner(problem, config, methods);
    runner.runMethods();
    return 0;
}
