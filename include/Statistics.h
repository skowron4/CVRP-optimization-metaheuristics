#ifndef STATISTICS_H
#define STATISTICS_H

#include "Individual.h"
#include <vector>
#include <ostream>

using namespace std;

class Statistics {
private:
    struct FitnessRecord {
        int best = 0, worst = 0;
        double avg = 0.0, stdDev = 0.0;

        friend ostream &operator<<(ostream &os, const FitnessRecord &record) {
            os << record.best << ',' << record.avg << ',' << record.worst << ',' << record.stdDev;
            return os;
        }

        static string getHeader() {
            return "best,avg,worst,stdDev";
        }
    };

    vector<FitnessRecord> records;

public:
    explicit Statistics(int iterations) {
        records.reserve(iterations);
    };

//    void initialize(int iterations);

//    void clear();

    void calculateAndAddStatisticsFitnessRecord(const vector<Individual> &individuals);

    static FitnessRecord calculateFitnessRecord(const vector<Individual> &individuals);

    void saveToFile(const string &filename) const;
};

#endif //STATISTICS_H
