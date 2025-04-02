#include <mutex>
#include <thread>
#include <fstream>
#include <filesystem>
#include "Methods.h"
#include "Utils.h"

vector<Individual> Method::generateNeighbourhood(const Individual &individual, Mutation &mutation, int size) {
    vector<Individual> newNeighbourhood(size, individual);

    for (Individual &ind: newNeighbourhood) ind.mutate(mutation);

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
    for (auto &thread: threads) thread.join();

    return individuals;
}

void Method::runEachMethodManyTimes(const vector<Method *> &methods, int numberOfRuns, vector<vector<Individual>> &results) {
    mutex mtx;

    auto runMethodManyTimes = [&results, &mtx, numberOfRuns](Method *method, int index) {
        vector<Individual> result = method->runManyTimes(numberOfRuns);
        lock_guard<mutex> lock(mtx);
        results[index] = std::move(result);
    };

    vector<thread> methodThreads;
    methodThreads.reserve(methods.size());

    for (int i = 0; i < methods.size(); ++i) methodThreads.emplace_back(runMethodManyTimes, methods[i], i);
    for (auto &thread: methodThreads) thread.join();
}

void Method::runEachMethodManyTimesAndSave(const Problem &problem, const vector<Method *> &methods, int numberOfRuns) {
    vector<vector<Individual>> results(methods.size());
    runEachMethodManyTimes(methods, numberOfRuns, results);

    // Save results to CSV file
    string filename = problem.getName();
    for (auto &method: methods) filename += "_" + method->short_name;

    string folder = "../data/results/box/";
    string filepath = folder + filename + "_" + getCurrentTimestamp() + ".csv";

    // Create folder if it does not exist
    filesystem::create_directories(folder);

    ofstream file(filepath);

    if (!file.is_open()) {
        cerr << "Could not open file " << filepath << endl;
        return;
    }

    // Write header
    string header;
    for (auto &method: methods) {
        header += method->short_name;
        if (&method != &methods.back()) header += ",";
    }
    file << header << "\n";

    // Write results
    for (int i = 0; i < numberOfRuns; ++i) {
        for (int j = 0; j < methods.size(); ++j) {
            file << results[j][i].getFitness();
            if (j != methods.size() - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
}

void Method::runEachMethodOnceAndSave(const vector<Method *> &methods) {
    auto runAndSave = [](Method *method) {
        unique_ptr<Method> methodClone = method->clone();
        methodClone->runAndSave();
    };

    vector<thread> threads;
    threads.reserve(methods.size());
    for (auto method: methods) threads.emplace_back(runAndSave, method);
    for (auto &thread: threads) thread.join();
}
