#include "Methods.h"

bool SimulatedAnnealingMethod::annealing(int best, int current) {
    return exp((best - current) / current_temperature) < real_dist(random_engine);
}

Individual* SimulatedAnnealingMethod::findBestIndividual(vector<Individual> &neighborhood) {
    Individual *bestInd = nullptr;

    for (auto& ind : neighborhood)
        if (bestInd == nullptr || *bestInd > ind) bestInd = &ind;

    return bestInd;
}

void SimulatedAnnealingMethod::algorithmStep(Individual &currentIndividual,
                                             Individual &bestIndividual,
                                             vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentIndividual, mutation, neighbourhood_size);
    auto tempInd = findBestIndividual(neighborhood);

    if (!tempInd) return;
    currentIndividual = *tempInd;

    if (currentIndividual < bestIndividual ||
        annealing(bestIndividual.getFitness(), currentIndividual.getFitness()))
            bestIndividual = currentIndividual;

    if (current_temperature > final_temperature) {
        current_temperature = cooling_scheme(current_temperature);
        current_temperature = std::max(current_temperature, final_temperature);
    }
}

Individual SimulatedAnnealingMethod::run() {
    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    Individual bestIndividual = currentIndividual;
    vector<Individual> neighborhood;

    current_temperature = initial_temperature;

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