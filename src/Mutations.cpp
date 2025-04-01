#include <algorithm>
#include <iostream>
#include "Mutations.h"

void SingleSwapMutation::mutate(vector<int> &genotype) {
    if (genotype.size() < 2) return;

    uniform_int_distribution<int> int_dist(0, genotype.size() - 1);
    int left = int_dist(random_engine);
    int right;

    // continue IF (genotype[left] == 0 and genotype[right]) == 0 OR (right == left)
    do right = int_dist(random_engine);
    while (genotype[left] == genotype[right]);

    swap(genotype[left], genotype[right]);
}

void InversionMutation::mutate(vector<int> &genotype) {
    if (genotype.size() < 2) return;

    uniform_int_distribution<int> int_dist(0, genotype.size() - 1);
    int left = int_dist(random_engine);
    int right;

    do {
        right = int_dist(random_engine);
        if (left > right) swap(left, right);
    } while (
            genotype[left] == genotype[right] ||
            (left > 0 && genotype[left - 1] == 0) && (right < genotype.size() - 1 && genotype[right + 1] == 0));

    reverse(genotype.begin() + left, genotype.begin() + right + 1);
}
