#ifndef PROBLEMCVRP_H
#define PROBLEMCVRP_H

#include "Loader.h"
#include <vector>
#include <random>

using namespace std;

class Individual;

class Problem {
private:
    Loader::Data data;

    vector<vector<int>> distance_matrix;
    int genotype_size, cities_count;

    void computeDistanceMatrix();

    Individual createGreedyIndividual(int startingCity);

public:
    explicit Problem(const Loader::Data& data) : data(data) {
        genotype_size = 2 * data.dimension - 3;
        cities_count = data.dimension - 1;
        computeDistanceMatrix();
    };

    int evaluateGenotype(const vector<int> &genotype) const;

    Individual createRandomIndividual(mt19937 &random_engine);

    vector<Individual> createGreedyIndividuals();

    void printDistanceMatrix() const;

    string getName() const { return data.name; }
};

#endif //PROBLEMCVRP_H
