#include <mutex>
#include <thread>
#include <fstream>
#include <filesystem>
#include "Methods.h"
#include "Utils.h"

vector<Individual> Method::generateNeighbourhood(const Individual &individual, Mutation &mutation, int size) {
    vector<Individual> newNeighbourhood(size, individual);

    for(Individual ind : newNeighbourhood) ind.mutate(mutation);

    return std::move(newNeighbourhood);
}

vector<Individual> Method::runManyTimes(int numberOfRuns) {
    vector<Individual> individuals;
    individuals.reserve(numberOfRuns);
    mutex mtx;

    auto runSingleInstance = [this, &individuals, &mtx] {
        unique_ptr<Method> methodCopy(this->clone());
        Individual result = methodCopy->run();
        lock_guard<mutex> lock(mtx);
        individuals.push_back(result);
    };

    vector<thread> threads;
    threads.reserve(numberOfRuns);
    for (int i = 0; i < numberOfRuns; ++i) threads.emplace_back(runSingleInstance);
    for (auto &thread : threads) thread.join();

    return individuals;
}

void Method::runEachMethodManyTimesAndSave(Problem &problem,
                                           unordered_map<string, unique_ptr<Method>> &methods,
                                           int numberOfRuns) {
    vector<vector<Individual>> results(methods.size());
    mutex mtx;

    auto runMethodManyTimes = [&results, &mtx, numberOfRuns](Method *method, int index) {
        vector<Individual> result = method->runManyTimes(numberOfRuns);
        lock_guard<mutex> lock(mtx);
        results[index] = result;
    };

    vector<thread> methodThreads;
    methodThreads.reserve(methods.size());
    int i{0};
    for (auto &pair : methods) methodThreads.emplace_back(runMethodManyTimes, pair.second.get(), i++);
    for (auto &thread : methodThreads) thread.join();

    // Save results to CSV file
    string filename = problem.getName();
    for (auto &method : methods) filename += "_" + method.second->short_name;

    string folder = "./data/results/box/";
    string filepath = folder + filename + "/" + getCurrentTimestamp() + ".csv";

    // Create folder if it does not exist
    filesystem::create_directories(folder);

    ofstream file(filepath);

    if (!file.is_open()) {
        cerr << "Could not open file " << filepath << endl;
        return;
    }

    // Write header
    string header;
    for (auto &method : methods) header += method.second->short_name + ",";
    file << "\n";

    // Write results
    for (int i = 0; i < numberOfRuns; ++i) {
        for (int j = 0; j < methods.size(); ++j) file << results[j][i].getFitness() << ",";
        file << "\n";
    }

    file.close();
}
