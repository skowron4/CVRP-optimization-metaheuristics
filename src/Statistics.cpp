#include "Statistics.h"
#include "Utils.h"
#include <algorithm>
#include <fstream>
#include <filesystem>

void Statistics::calculateAndAddStatisticsFitnessRecord(const vector<Individual> &individuals) {
    records.push_back(calculateFitnessRecord(individuals));
}

Statistics::FitnessRecord Statistics::calculateFitnessRecord(const vector<Individual> &individuals) {
    FitnessRecord record;

    int individualsSize = individuals.size();

    auto minmax = minmax_element(individuals.begin(), individuals.end());
    record.best = minmax.first->getFitness();
    record.worst = minmax.second->getFitness();

    int sum = accumulate(individuals.begin(), individuals.end(), 0, [](int acc, const Individual &individual) {
        return acc + individual.getFitness();
    });
    record.avg = static_cast<double>(sum) / individualsSize;

    double variance = accumulate(individuals.begin(), individuals.end(), 0.0, [&](double acc, const Individual &individual) {
        return acc + pow(individual.getFitness() - record.avg, 2);
    });
    record.stdDev = sqrt(variance / individualsSize);

    return record;
}

void Statistics::saveToFile(const string &filename) const {
    string folder = "../data/results/single/";
    string filepath = folder + filename + "_" + getCurrentTimestamp() + ".csv";

    // Create folder if it does not exist
    filesystem::create_directories(folder);

    ofstream file(filepath);

    if (!file.is_open()) {
        cerr << "Could not open file " << filepath << endl;
        return;
    }

    file << FitnessRecord::getHeader() << endl;
    for (const auto &record: records) file << record << endl;

    file.close();
}
