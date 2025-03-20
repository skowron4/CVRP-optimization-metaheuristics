#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "Mutations.h"
#include "Problem.h"
#include <vector>
#include <iomanip>
#include <iostream>

using namespace std;

class Problem;

class Individual {
private:
    Problem &problem;
    vector<int> genotype;
    mutable int fitness;
    mutable bool is_evaluated;

public:
    Individual(Problem &problem, const vector<int> &genotype) :
            problem(problem),
            genotype(genotype),
            fitness(0),
            is_evaluated(false) {};

    Individual(const Individual &other) :
            problem(other.problem),
            is_evaluated(false),
            genotype(other.genotype),
            fitness(other.fitness) {};

    Individual &operator=(const Individual &other) {
        if (this != &other) {
            problem = other.problem;
            genotype = other.genotype;
            fitness = other.fitness;
            is_evaluated = other.is_evaluated;
        }
        return *this;
    }

    struct Hash {
        size_t operator()(const Individual &individual) const {
            return std::hash<int>()(individual.getFitness());
        }
    };

    struct Equal {
        bool operator()(const Individual &lhs, const Individual &rhs) const {
            return lhs.getFitness() == rhs.getFitness() && lhs.genotype == rhs.genotype;
        }
    };

    friend ostream &operator<<(ostream &os, const Individual &individual);

    bool operator>(const Individual &other) const;

    bool operator<(const Individual &other) const;

    bool operator==(const Individual &other) const;

    int getFitness() const;

    void mutate(Mutation &mutation);
};

#endif //INDIVIDUAL_H
