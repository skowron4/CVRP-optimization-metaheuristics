#include "Individual.h"

ostream &operator<<(ostream &os, const Individual &individual) {
    for (int gene: individual.genotype) os << setw(4) << gene;
    return os;
}

bool Individual::operator>(const Individual &other) const {
    return getFitness() > other.getFitness();
}

bool Individual::operator<(const Individual &other) const {
    return getFitness() < other.getFitness();
}

bool Individual::operator==(const Individual &other) const {
    return getFitness() == other.getFitness() && genotype == other.genotype;
}

int Individual::getFitness() const {
    if (!is_evaluated) {
        fitness = problem.evaluateGenotype(genotype);
        is_evaluated = true;
    }
    return fitness;
}

void Individual::mutate(Mutation &mutation) {
    mutation.mutate(genotype);
    is_evaluated = false;
}
