#include <algorithm>
#include "Methods.h"

Individual* TabuSearchMethod::findBestIndividual(vector<Individual> &neighborhood) {
   Individual *bestInd = nullptr;

    for (auto& ind : neighborhood)
        if (!tabu_list.contains(ind) && (bestInd == nullptr || *bestInd > ind)) bestInd = &ind;

    return bestInd;
}

void TabuSearchMethod::algorithmStep(Individual &currentIndividual,
                                     Individual &bestIndividual,
                                     vector<Individual> &neighborhood) {

    neighborhood = generateNeighbourhood(currentIndividual, mutation, neighbourhood_size);
    auto temp = findBestIndividual(neighborhood);

    if (temp) currentIndividual = *temp;

    if (currentIndividual < bestIndividual) bestIndividual = currentIndividual;
    tabu_list.add(currentIndividual);
}

Individual TabuSearchMethod::run() {
    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    Individual bestIndividual = currentIndividual;
    vector<Individual> neighborhood;
    tabu_list.add(currentIndividual);

    for (int i = 0; i < iterations; ++i) algorithmStep(currentIndividual, bestIndividual, neighborhood);

    return bestIndividual;
}

Individual TabuSearchMethod::runAndSave() {
    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    Individual bestIndividual = currentIndividual;
    vector<Individual> neighborhood;
    tabu_list.add(currentIndividual);

    Statistics statistics(iterations);

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentIndividual, bestIndividual, neighborhood);
        statistics.calculateAndAddStatisticsFitnessRecord(neighborhood);
    }

    statistics.saveToFile("placeHolderFile");

    return bestIndividual;
}