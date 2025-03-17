#ifndef PROBLEMCVRP_H
#define PROBLEMCVRP_H

#include "Individual.h"
#include "Loader.h"
#include <vector>
#include <random>

using namespace std;

class Individual;

class Problem {
private:
    Loader::Data data;

    vector<vector<int>> distance_matrix;
    int genotype_size, cities_count, depots_count;

    void computeDistanceMatrix();

    Individual createGreedyIndividual(int startingCity);

public:
    explicit Problem(const Loader::Data& data) : data(data) {
        genotype_size = 2 * data.dimension - 3;
        cities_count = data.dimension - 1;
        depots_count = cities_count - 1;
        computeDistanceMatrix();
    };

    int evaluateGenotype(const vector<int> &genotype) const;

    Individual createRandomIndividual(mt19937 &random_engine);

    vector<Individual> createGreedyIndividuals();

    bool isFirstFitnessBetter(int firstFitness, int secondFitness) const;

    void printData() const;

    void printDistanceMatrix() const;
};

#endif //PROBLEMCVRP_H
