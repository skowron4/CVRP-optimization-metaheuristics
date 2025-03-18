#include <algorithm>
#include "Mutations.h"

void SingleSwapMutation::mutate(vector<int> &genotype) {
    if (genotype.size() < 2) return;

    updateIndices(genotype.size());

    vector<int> chosen(2);
    sample(indices.begin(), indices.end(), chosen.begin(), 2, random_engine);

    swap(genotype[chosen[0]], genotype[chosen[1]]);
}

void InversionMutation::mutate(vector<int> &genotype) {
    if (genotype.size() < 2) return;

    updateIndices(genotype.size());

    vector<int> chosen(2);
    sample(indices.begin(), indices.end(), chosen.begin(), 2, random_engine);

    int left = min(chosen[0], chosen[1]);
    int right = max(chosen[0], chosen[1]);

    reverse(genotype.begin() + left, genotype.begin() + right + 1);
}