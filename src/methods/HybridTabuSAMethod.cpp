#include "Methods.h"
#include "Utils.h"

bool HybridTabuSAMethod::annealing(int current) {
    return exp((best_individual.getFitness() - current) / current_temperature) < real_dist(random_engine);
}

bool HybridTabuSAMethod::isBest(Individual &ind, Individual *bestInd) {
    return !tabu_list.contains(ind) && (bestInd == nullptr || *bestInd > ind || annealing(ind.getFitness()));
}

bool HybridTabuSAMethod::cooling() {
    current_temperature = std::max(cooling_scheme(current_temperature, cooling_ratio), final_temperature);
    return current_temperature > final_temperature;
}

bool HybridTabuSAMethod::heating() {
    current_temperature = std::min(heating_scheme(current_temperature, heating_ratio), max_heating_temperature);
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

Individual* HybridTabuSAMethod::findBestIndividual(vector<Individual> &individuals) {
    Individual *bestInd = nullptr;

    for (auto& ind : individuals) if (isBest(ind, bestInd)) bestInd = &ind;

    return bestInd;
}

void HybridTabuSAMethod::algorithmStep(Individual &currentIndividual, vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentIndividual, mutation, neighbourhood_size);
    auto tempInd = findBestIndividual(neighborhood);

    if (!tempInd) return;
    currentIndividual = *tempInd;

    if (currentIndividual < best_individual) {
        best_individual = currentIndividual;
        tabu_list.add(currentIndividual);
    }
}

Individual HybridTabuSAMethod::run() {
    reset();

    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    best_individual = currentIndividual;
    vector<Individual> neighborhood;

    tabu_list.add(currentIndividual);

    bool isCooling = true;
    int iterationCounterToHeating = iteration_to_start_heating;

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentIndividual, neighborhood);
        updateTemperature(isCooling, iterationCounterToHeating);
    }

    return best_individual;
}

Individual HybridTabuSAMethod::runAndSave() {
    reset();

    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    best_individual = currentIndividual;
    vector<Individual> neighborhood;

    tabu_list.add(currentIndividual);

    bool isCooling = true;
    int iterationCounterToHeating = iteration_to_start_heating;

    Statistics statistics(iterations);

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentIndividual, neighborhood);
        updateTemperature(isCooling, iterationCounterToHeating);
        statistics.calculateAndAddStatisticsFitnessRecord(neighborhood);
    }

    statistics.saveToFile(getFileName());

    return best_individual;
}

string HybridTabuSAMethod::getFileName() const {
    return problem.getName() + "_" + short_name + "_" +
           "mut_" + mutation.getName() + "_" +
           "iter_" + to_string(iterations) + "_" +
           "tabu_" + to_string(tabu_list_size) + "_" +
           "neigh_" + to_string(neighbourhood_size) + "_" +
           "T0_" + doubleToString(initial_temperature) + "_" +
           "Tf_" + doubleToString(final_temperature) + "_" +
           "maxTh_" + doubleToString(max_heating_temperature) + "_" +
           "iterHeat_" + to_string(iteration_to_start_heating) + "_" +
           "coolScheme_" + cool_scheme_name + "_" +
           "coolRatio_" + to_string(cooling_ratio)  + "_" +
           "heatScheme_" + heat_scheme_name + "_" +
           "heatRatio_" + to_string(heating_ratio);
}

void HybridTabuSAMethod::reset() {
    tabu_list.clear();
    current_temperature = initial_temperature;
}
