#include <iostream>
#include <json.hpp>
#include <fstream>
#include "Methods.h"
#include "JSONParser.h"
#include "MethodRunner.h"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);

    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <config_file_path>" << endl;
        return 0;
    }

    string config_file_path = argv[1];
    string data_file_path = argv[2];

    Loader loader;
    auto data = loader.loadProblemFromFile(data_file_path);
    if (!data.has_value()) {
        cerr << "Error while loading data from file" << endl;
        return 1;
    }

    Problem problem(data.value());

    JSONParser json_parser(config_file_path);

    MethodRunner runner(problem, json_parser);
    runner.runConfig();

    return 0;
}
