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

    record.best = min_element(individuals.begin(), individuals.end())->getFitness();
    record.worst = max_element(individuals.begin(), individuals.end())->getFitness();

    record.avg = static_cast<double>(accumulate(individuals.begin(), individuals.end(), 0, [](int sum, const Individual &individual) {
        return sum + individual.getFitness();
    })) / individuals.size();

    double variance = accumulate(individuals.begin(), individuals.end(), 0.0, [&](double sum, const Individual &individual) {
        return sum + pow(individual.getFitness() - record.avg, 2);
    });
    record.stdDev = sqrt(variance / individuals.size());

    return record;
}

void Statistics::saveToFile(const string &filename) const {
    string folder = "./data/results/single/";
    string filepath = folder + filename + "/" + getCurrentTimestamp() + ".csv";

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
