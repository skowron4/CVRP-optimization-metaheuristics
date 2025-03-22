#include "Methods.h"

void SimulatedAnnealingMethod::algorithmStep(Individual &currentIndividual,
                                             Individual &bestIndividual,
                                             vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentIndividual, mutation, neighbourhood_size);

    auto tempInd = findBestIndividual(neighborhood);

    if (tempInd) currentIndividual = *tempInd;
    else if (currentIndividual < bestIndividual) bestIndividual = currentIndividual;
}

Individual* SimulatedAnnealingMethod::findBestIndividual(vector<Individual> &neighborhood) {
    Individual *bestInd = nullptr;

    for (auto& ind : neighborhood)
        if (bestInd == nullptr || *bestInd > ind) bestInd = &ind;

    return bestInd;
}

Individual SimulatedAnnealingMethod::run() {
    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    Individual bestIndividual = currentIndividual;
    vector<Individual> neighborhood;

    for (int i = 0; i < iterations; ++i)
        algorithmStep(currentIndividual, bestIndividual, neighborhood);

    return bestIndividual;
}

Individual SimulatedAnnealingMethod::runAndSave() {
    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    Individual bestIndividual = currentIndividual;
    vector<Individual> neighborhood;

    Statistics statistics(iterations);

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentIndividual, bestIndividual, neighborhood);
        statistics.calculateAndAddStatisticsFitnessRecord(neighborhood);
    }

    statistics.saveToFile("placeHolderFile");

    return bestIndividual;
}