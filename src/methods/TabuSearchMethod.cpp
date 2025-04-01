#include <algorithm>
#include "Methods.h"

Individual& TabuSearchMethod::findBestIndividual(vector<Individual> &individuals, Individual &currentInd) {
    Individual *best = nullptr;

    for (Individual &ind : individuals)
        if (!tabu_list.contains(ind) && (best == nullptr || *best > ind)) best = &ind;

    return best ? *best : currentInd;
}

void TabuSearchMethod::algorithmStep(Individual &currentIndividual, vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentIndividual, mutation, neighbourhood_size);
    currentIndividual = findBestIndividual(neighborhood, currentIndividual);

    if (currentIndividual < best_individual) {
        best_individual = currentIndividual;
        tabu_list.add(currentIndividual);
    }
}

Individual TabuSearchMethod::run() {
    reset();

    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    best_individual = currentIndividual;
    vector<Individual> neighborhood;

    tabu_list.add(currentIndividual);

    for (int i = 0; i < iterations; ++i) algorithmStep(currentIndividual, neighborhood);

    return best_individual;
}

Individual TabuSearchMethod::runAndSave() {
    reset();

    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    best_individual = currentIndividual;
    vector<Individual> neighborhood;

    tabu_list.add(currentIndividual);

    Statistics statistics(iterations);

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentIndividual, neighborhood);
        statistics.calculateAndAddStatisticsFitnessRecord(neighborhood);
    }

    statistics.saveToFile(getFileName());

    return best_individual;
}

string TabuSearchMethod::getFileName() const {
    return problem.getName() + "_" + short_name + "_" +
           "mut_" + mutation.getName() + "_" +
           "iter_" + to_string(iterations) + "_" +
           "tabu_" + to_string(tabu_list_size) + "_" +
           "neigh_" + to_string(neighbourhood_size);
}

void TabuSearchMethod::reset() {
    tabu_list.clear();
}
