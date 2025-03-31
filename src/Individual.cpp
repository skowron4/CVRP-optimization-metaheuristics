#include "Individual.h"

ostream &operator<<(ostream &os, const Individual &individual) {
    for (int gene: individual.genotype) os << setw(4) << gene;
    return os;
}

bool Individual::operator>(const Individual &other) const {
    return getFitness() > other.getFitness();
}

bool Individual::operator>=(const Individual &other) const {
    return getFitness() >= other.getFitness();
}

bool Individual::operator<(const Individual &other) const {
    return getFitness() < other.getFitness();
}

bool Individual::operator<=(const Individual &other) const {
    return getFitness() <= other.getFitness();
}

bool Individual::operator==(const Individual &other) const {
    return (is_evaluated && other.is_evaluated
    && fitness == other.fitness && compareGenotype(genotype, other.genotype))
    || compareGenotype(genotype, other.genotype);
}

int Individual::getFitness() const {
    if (!is_evaluated) {
        fitness = problem.evaluateGenotype(genotype);
        is_evaluated = true;
    }
    return fitness;
}

bool Individual::compareGenotype(const vector<int>& vec1, const vector<int>& vec2) const {
    return simplifyVector(vec1) == simplifyVector(vec2);
}

vector<int> Individual::simplifyVector(const vector<int>& vec) const {
    vector<int> simplified;
    bool zeroFlag = false;

    for (int num : vec) {
        if (num == 0) {
            if (!zeroFlag) {
                simplified.push_back(0);
                zeroFlag = true;
            }
        } else {
            simplified.push_back(num);
            zeroFlag = false;
        }
    }
    return simplified;
}

void Individual::mutate(Mutation &mutation) {
    mutation.mutate(genotype);
    is_evaluated = false;
}

void Individual::printGenotype() {
    std::cout << "Genotype: " << getFitness() << std::endl;
    for (int i: genotype) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
