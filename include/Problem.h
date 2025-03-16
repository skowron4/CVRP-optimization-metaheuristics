#ifndef PROBLEMCVRP_H
#define PROBLEMCVRP_H

#include "Loader.h"
#include <vector>

using namespace std;

class Individual;

class Problem {
private:
    Loader::Data data;

    vector<vector<int>> distance_matrix;
    int genotype_size, cities_count, depots_count;

    void computeDistanceMatrix();

    Individual createGreedyIndividual(int startingCity) const;

public:
    explicit Problem(const Loader::Data data) : data(data) {};

    int evaluateGenotype(const vector<int> &genotype) const;

    Individual createRandomIndividual(mt19937 &random_engine) const;

    vector<Individual> createGreedyIndividuals() const;

    bool isFirstFitnessBetter(int firstFitness, int secondFitness) const;

    void printDistanceMatrix() const;
};

#endif //PROBLEMCVRP_H
