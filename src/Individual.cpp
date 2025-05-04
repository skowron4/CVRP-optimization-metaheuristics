#include "Individual.h"
#include <algorithm>

ostream &operator<<(ostream &os, const Individual &individual) {
    for (int gene: individual.genotype) os << gene << " ";
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
    return getFitness() == other.getFitness() &&
    compareGenotype(genotype, other.genotype);
}

int Individual::getFitness() const {
    if (!is_evaluated) {
        fitness = problem.evaluateGenotype(genotype);
        is_evaluated = true;
    }
    return fitness;
}

bool Individual::compareGenotype(const vector<int>& vec1, const vector<int>& vec2) const {
    auto first_non_zero_vec1 = find_if(vec1.begin(), vec1.end(), [](int i) { return i != 0; });
    auto last_non_zero_vec1 = find_if(vec1.rbegin(), vec1.rend(), [](int i) { return i != 0; }).base();

    auto first_non_zero_vec2 = find_if(vec2.begin(), vec2.end(), [](int i) { return i != 0; });
    auto last_non_zero_vec2 = find_if(vec2.rbegin(), vec2.rend(), [](int i) { return i != 0; }).base();

    vector simplified_vec1(first_non_zero_vec1, last_non_zero_vec1);
    vector simplified_vec2(first_non_zero_vec2, last_non_zero_vec2);

    auto last_vec1 = unique(simplified_vec1.begin(), simplified_vec1.end());
    auto last_vec2 = unique(simplified_vec2.begin(), simplified_vec2.end());

    return equal(simplified_vec1.begin(), last_vec1 - 1, simplified_vec2.begin(), last_vec2 - 1);
}

void Individual::mutate(Mutation &mutation) {
    mutation.mutate(genotype);
    is_evaluated = false;
}

void Individual::printGenotype() const {
    cout << "Genotype: " << getFitness() << endl;
    cout << *this << endl;
}
