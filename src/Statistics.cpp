#include "Statistics.h"
#include <algorithm>
#include <fstream>

//void Statistics::initialize(int iterations) {
//    records.clear();
//    records.reserve(iterations);
//}

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

//void Statistics::clear() {
//    records.clear();
//}

void Statistics::saveToFile(const string &filename) const {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Could not open file " << filename << endl;
        return;
    }

    file << FitnessRecord::getHeader() << endl;
    for (const auto &record: records) file << record << endl;

    file.close();
}
