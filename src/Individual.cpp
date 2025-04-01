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
    return simplifyVector(vec1) == simplifyVector(vec2);
}

//vector<int> Individual::simplifyVector(const vector<int>& vec) const {
//    vector<int> simplified;
//    bool zeroFlag = false;
//
//    for (int num : vec) {
//        if (num == 0) {
//            if (!zeroFlag) {
//                simplified.push_back(0);
//                zeroFlag = true;
//            }
//        } else {
//            simplified.push_back(num);
//            zeroFlag = false;
//        }
//    }
//    return simplified;
//}

vector<int> Individual::simplifyVector(const vector<int>& vec) const {
    auto first_non_zero = find_if(vec.begin(), vec.end(), [](int i) { return i != 0; });
    auto last_non_zero = find_if(vec.rbegin(), vec.rend(), [](int i) { return i != 0; }).base();

    vector<int> simplified(first_non_zero, last_non_zero);
    auto last = unique(simplified.begin(), simplified.end());
    simplified.erase(last, simplified.end());

    return simplified;
}

void Individual::mutate(Mutation &mutation) {
    mutation.mutate(genotype);
    is_evaluated = false;
}

void Individual::printGenotype() const {
    cout << "Genotype: " << getFitness() << endl;
    cout << *this << endl;
}
