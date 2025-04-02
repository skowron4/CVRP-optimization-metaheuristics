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

//void Statistics::saveToFile(const string &filename) const {
//    string folder = "../data/results/single/";
//    string filepath = folder + filename + "_" + getCurrentTimestamp() + ".csv";
//
//    // Create folder if it does not exist
//    filesystem::create_directories(folder);
//
//    ofstream file(filepath);
//
//    if (!file.is_open()) {
//        cerr << "Could not open file " << filepath << endl;
//        return;
//    }
//
//    file << FitnessRecord::getHeader() << endl;
//    for (const auto &record: records) file << record << endl;
//
//    file.close();
//}

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

    ostringstream buffer;
    int size_to_reserve = determineBufferSize(buffer.precision());
    buffer.str().reserve(size_to_reserve);

    buffer << FitnessRecord::HEADER << '\n';
    for (const auto &record: records) buffer << record << '\n';

    file << buffer.str();
    file.close();
}

int Statistics::determineBufferSize(int doublePrecision) const {
    int worst = max_element(records.begin(), records.end(), [](const auto &record1, const auto &record2) {
        return record1.worst < record2.worst;
    })->worst;

    int header_length = FitnessRecord::HEADER.length();

    int max_int_length = to_string(worst).length();
    int max_double_length = max_int_length + doublePrecision + 1; // +1 for the decimal point

    int record_length = (max_int_length + max_double_length) * 2 + 3; // 2 integers + 2 doubles + 3 commas

    int new_line_count = records.size() + 1; // +1 for the header

    return header_length + records.size() * record_length + new_line_count;
}
