#include <mutex>
#include <thread>
#include <fstream>
#include <filesystem>
#include "Methods.h"
#include "Utils.h"

vector<Individual> Method::generateNeighbourhood(const Individual &individual, int size) {
    vector<Individual> newNeighbourhood(size, individual);

    for (Individual &ind: newNeighbourhood) ind.mutate(*mutation);

    return std::move(newNeighbourhood);
}

vector<vector<Individual>> Method::runEachMethodBoxPlot(const vector<vector<unique_ptr<Method>>> &methods) {
    vector<vector<Individual>> results(methods.size());
    for (int i = 0; i < methods.size(); ++i) results[i].reserve(methods[i].size());

    mutex mtx;

    auto runMethod = [&results, &mtx](Method *method, int index) {
        Individual result = method->run();
        lock_guard<mutex> lock(mtx);
        results[index].push_back(result);
    };

    vector<thread> threads;
    threads.reserve(methods.size() * methods[0].size());

    for (int i = 0; i < methods.size(); ++i)
        for (const auto &method: methods[i])
            threads.emplace_back(runMethod, method.get(), i);

    for (auto &thread: threads) thread.join();

    return results;
}

void Method::runEachMethodAndSaveBoxPlot(const Problem &problem, const vector<vector<unique_ptr<Method>>> &methods) {
    int numberOfRuns = methods[0].size();
    if (numberOfRuns == 0) return;

    vector<vector<Individual>> results = runEachMethodBoxPlot(methods);

    // Save results to CSV file
    string filename = problem.getName();
    for (const auto & method : methods) filename += "_" + method[0]->type;
    filename += "_iter_" + to_string(numberOfRuns);

    string folder = "../data/results/box/";
    string filepath = folder + filename + "_" + getCurrentTimestamp() + ".csv";

    // Create folder if it does not exist
    filesystem::create_directories(folder);

    ofstream file(filepath);

    if (!file.is_open()) {
        cerr << "Could not open file " << filepath << endl;
        return;
    }

    // Approximate buffer size assuming 10 characters per method type, 10 characters per number, commas, and newlines
    ostringstream buffer;
    int bufferSize = methods.size() * 10 + numberOfRuns * methods.size() * 11;
    buffer.str().reserve(bufferSize);

    // Write header
    for (int j = 0; j < methods.size() - 1; ++j)
        buffer << methods[j][0]->type << ",";
    buffer << methods[methods.size() - 1][0]->type << "\n";

    // Write results
    for (int i = 0; i < numberOfRuns; ++i) {
        for (int j = 0; j < methods.size() - 1; ++j)
            buffer << results[j][i].getFitness() << ",";
        buffer << results[methods.size() - 1][i].getFitness() << "\n";
    }

    file << buffer.str();

    file.close();
}

void Method::runEachMethodAndSaveSinglePlots(const vector<unique_ptr<Method>> &methods) {
    vector<thread> threads;
    threads.reserve(methods.size());
    for (const auto &method: methods) { threads.emplace_back(&Method::runAndSave, method.get()); }
    for (auto &thread: threads) thread.join();
}
