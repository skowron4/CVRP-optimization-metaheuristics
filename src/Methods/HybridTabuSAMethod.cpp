#include "Methods.h"

bool HybridTabuSAMethod::annealing(int current) {
    return exp((best_score - current) / current_temperature) < real_dist(random_engine);
}

bool HybridTabuSAMethod::isBest(Individual &ind, Individual *bestInd) {
    return !tabu_list.contains(ind) && (bestInd == nullptr || *bestInd > ind || annealing(ind.getFitness()));
}

bool HybridTabuSAMethod::cooling() {
    current_temperature = std::max(cooling_scheme(current_temperature), final_temperature);
    return current_temperature > final_temperature;
}

bool HybridTabuSAMethod::heating() {
    current_temperature = std::min(heating_scheme(current_temperature), max_heating_temperature);
    return current_temperature == max_heating_temperature;
}

void HybridTabuSAMethod::updateTemperature(bool &isCooling, int &iterToChange) {
    if (isCooling) {
        if (iterToChange == 0) iterToChange = iteration_to_start_heating;
        isCooling = cooling();
    }
    else if (iterToChange > 0) iterToChange--;
    else isCooling = heating();
}

Individual* HybridTabuSAMethod::findBestIndividual(vector<Individual> &neighborhood) {
    Individual *bestInd = nullptr;

    for (auto& ind : neighborhood) if (isBest(ind, bestInd)) bestInd = &ind;

    return bestInd;
}

void HybridTabuSAMethod::algorithmStep(Individual &currentIndividual,
                                       Individual &bestIndividual,
                                       vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentIndividual, mutation, neighbourhood_size);
    auto tempInd = findBestIndividual(neighborhood);

    if (!tempInd) return;
    currentIndividual = *tempInd;

    if (currentIndividual < bestIndividual) {
        bestIndividual = currentIndividual;
        best_score = bestIndividual.getFitness();
        tabu_list.add(currentIndividual);
    }
}

Individual HybridTabuSAMethod::run() {
    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    Individual bestIndividual = currentIndividual;
    vector<Individual> neighborhood;
    tabu_list.add(currentIndividual);
    current_temperature = initial_temperature;
    bool isCooling = true;
    int iterationFromCoolingCompletionTimeToHeating = iteration_to_start_heating;

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentIndividual, bestIndividual, neighborhood);
        updateTemperature(isCooling, iterationFromCoolingCompletionTimeToHeating);
    }

    return bestIndividual;
}

Individual HybridTabuSAMethod::runAndSave() {
    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    Individual bestIndividual = currentIndividual;
    vector<Individual> neighborhood;
    tabu_list.add(currentIndividual);
    current_temperature = initial_temperature;
    bool isCooling = true;
    int iterationFromCoolingCompletionTimeToHeating = iteration_to_start_heating;
    Statistics statistics(iterations);

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentIndividual, bestIndividual, neighborhood);
        updateTemperature(isCooling, iterationFromCoolingCompletionTimeToHeating);
        statistics.calculateAndAddStatisticsFitnessRecord(neighborhood);
    }

    statistics.saveToFile("placeHolderFile");

    return bestIndividual;
}