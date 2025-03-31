#include <algorithm>
#include <iostream>
#include "Mutations.h"

void SingleSwapMutation::mutate(vector<int> &genotype) {
    if (genotype.size() < 2) return;

    vector<int> chosen(2);

    do sample(indices.begin(), indices.end(), chosen.begin(), 2, random_engine);
    while (genotype[chosen[0]] == genotype[chosen[1]]);

    swap(genotype[chosen[0]], genotype[chosen[1]]);
}

void InversionMutation::mutate(vector<int> &genotype) {
    if (genotype.size() < 2) return;

    int left, right;
    std::uniform_int_distribution<int> int_dist(0, genotype.size() - 1);
    left = int_dist(random_engine);

    do {
        right = int_dist(random_engine);
        if (left > right) swap(left, right);
    } while (
            (genotype[left] == 0 && genotype[right] == 0) ||
            ((left > 0 && genotype[left - 1] == 0) && (right < genotype.size() - 1 && genotype[right + 1] == 0)) ||
            left == right);

    reverse(genotype.begin() + left, genotype.begin() + right + 1);
}
