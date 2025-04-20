#include <iostream>
#include <filesystem>
#include <json.hpp>
#include "Methods.h"
#include "JSONParser.h"
#include "MethodRunner.h"

using namespace std;
using namespace std::filesystem;
using json = nlohmann::json;

//TODO :

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <config_file_path> <data_file_path> [<destination_directory>]" << endl;
        return 1;
    }

    path config_file_path = argv[1];
    path data_file_path = argv[2];
    path destination_directory = argc > 3 ? argv[3] : current_path();

    try {
        config_file_path = canonical(config_file_path);
        data_file_path = canonical(data_file_path);
        destination_directory = canonical(destination_directory);
    }
    catch (const filesystem_error &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    Loader loader;
    auto dataOpt = loader.loadProblemFromFile(data_file_path);
    if (!dataOpt.has_value()) {
        cerr << "Error while loading data from file: " << data_file_path << endl;
        return 1;
    }

    Problem problem(dataOpt.value());

    JSONParser json_parser(config_file_path);

    MethodRunner runner(problem, json_parser);
    runner.runConfig(destination_directory);

    return 0;
}
