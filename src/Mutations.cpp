#include <algorithm>
#include "Mutations.h"

void Mutation::updateIndices(size_t genotype_size) {
    if (indices.size() != genotype_size) {
        indices.resize(genotype_size);
        iota(indices.begin(), indices.end(), 0);
    }
}

void SingleSwapMutation::mutate(vector<int> &genotype) {
    if (genotype.size() < 2) return;

    updateIndices(genotype.size());

    vector<int> chosen(2);

    do sample(indices.begin(), indices.end(), chosen.begin(), 2, random_engine);
    while (genotype[chosen[0]] == genotype[chosen[1]]);

    swap(genotype[chosen[0]], genotype[chosen[1]]);
}

void InversionMutation::mutate(vector<int> &genotype) {
    if (genotype.size() < 2) return;

    updateIndices(genotype.size());

    vector<int> chosen(2);
    int left, right;

    do {
        sample(indices.begin(), indices.end(), chosen.begin(), 2, random_engine);
        left = min(chosen[0], chosen[1]);
        right = max(chosen[0], chosen[1]);
    } while (all_of(genotype.begin() + left, genotype.begin() + right + 1, [](int val) { return val == 0; }));

    reverse(genotype.begin() + left, genotype.begin() + right + 1);
}
