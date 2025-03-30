#include <algorithm>
#include "Methods.h"

Individual *TabuSearchMethod::findCurrentBestIndividual(vector<Individual> &individuals) {
    Individual *bestInd = nullptr;

    for (auto &ind: individuals)
        if (!tabu_list.contains(ind) && (bestInd == nullptr || *bestInd > ind)) bestInd = &ind;

    return bestInd;
}

void TabuSearchMethod::algorithmStep(Individual &currentIndividual, vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentIndividual, mutation, neighbourhood_size);
    auto temp = findCurrentBestIndividual(neighborhood);

    if (!temp) return;
    currentIndividual = *temp;

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
