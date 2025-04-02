#ifndef STATISTICS_H
#define STATISTICS_H

#include "Individual.h"
#include <vector>
#include <ostream>

using namespace std;

class Statistics {
public:
    struct FitnessRecord {
        int best = 0, worst = 0;
        double avg = 0.0, stdDev = 0.0;

        inline static const string HEADER = "best,avg,worst,stdDev";

        friend ostream &operator<<(ostream &os, const FitnessRecord &record) {
            os << record.best << ',' << record.avg << ',' << record.worst << ',' << record.stdDev;
            return os;
        }
    };

private:
    vector<FitnessRecord> records;

    int determineBufferSize(int doublePrecision) const;

public:

    explicit Statistics(int iterations) {
        records.reserve(iterations);
    };

    void calculateAndAddStatisticsFitnessRecord(const vector<Individual> &individuals);

    static FitnessRecord calculateFitnessRecord(const vector<Individual> &individuals);

    void saveToFile(const string &filename) const;
};

#endif //STATISTICS_H
