#include <mutex>
#include <thread>
#include <fstream>
#include <filesystem>
#include "Methods.h"
#include "Utils.h"

vector<Individual> Method::generateNeighbourhood(const Individual &individual, Mutation &mutation, int size) {
    vector<Individual> newNeighbourhood(size, individual);

    for(Individual& ind : newNeighbourhood) ind.mutate(mutation);

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
    for (auto it = methods.begin(); it != methods.end(); ++it, ++i)
        methodThreads.emplace_back(runMethodManyTimes, it->second.get(), i);
    for (auto &thread : methodThreads) thread.join();

    // Save results to CSV file
    string filename = problem.getName();
    for (auto &method : methods) filename += "_" + method.second->short_name;

    string folder = "../data/results/box/";
    string filepath = folder + filename + "_" + getCurrentTimestamp() + ".csv";

    // Create folder if it does not exist
    filesystem::create_directories(folder);

    std::ofstream file("../data/results/box/A-n32-k5_TS_27-03-2025-18-10-12.csv");

    if (!file) {
        cerr << "Could not open file " << filepath << endl;
        return;
    }

    // Write header
    string header;
    for (auto it = methods.begin(); it != methods.end(); ++it) {
        if (it != methods.begin()) header += ",";
        header += it->second->short_name;
    }
    file << header << "\n";

    // Write results
    for (int i = 0; i < numberOfRuns; ++i) {
        for (int j = 0; j < methods.size(); ++j) {
            if (j == methods.size() - 1) file << results[j][i].getFitness();
            else file << results[j][i].getFitness() << ",";
        }
        file << "\n";
    }

    file.close();
}
