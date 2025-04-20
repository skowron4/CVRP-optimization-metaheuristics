#include "Methods.h"
#include "Utils.h"

bool HybridTabuSAMethod::annealing(double newIndScore, double oldIndScore) {
    return exp((oldIndScore - newIndScore) / current_temperature) > real_dist(random_engine);
}

bool HybridTabuSAMethod::cooling() {
    current_temperature = max(cooling_scheme(current_temperature, cooling_ratio), final_temperature);
    return current_temperature != final_temperature;
}

bool HybridTabuSAMethod::heating() {
    current_temperature = min(heating_scheme(current_temperature, heating_ratio), max_heating_temperature);
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

Individual& HybridTabuSAMethod::findBestIndividual(vector<Individual> &individuals, Individual &currentInd) {
    for (auto &ind: individuals)
        if (!tabu_list.contains(ind) &&
                (currentInd > ind || annealing(ind.getFitness(), currentInd.getFitness())))
            currentInd = ind;
    return currentInd;
}

void HybridTabuSAMethod::algorithmStep(Individual &currentBestIndividual, vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentBestIndividual, neighbourhood_size);
    currentBestIndividual = findBestIndividual(neighborhood, currentBestIndividual);

    if (currentBestIndividual < best_individual) {
        best_individual = currentBestIndividual;
        tabu_list.add(currentBestIndividual);
    }
}

Individual HybridTabuSAMethod::run() {
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

Individual HybridTabuSAMethod::runAndSave(const path &outDir) {
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

    statistics.saveToFile(getFileName(), outDir);

    return best_individual;
}

string HybridTabuSAMethod::getFileName() const {
    return problem.getName() + "_" + type + "_" +
           "mut_" + mutation->getType() + "_" +
           "iter_" + to_string(iterations) + "_" +
           "tabu_" + to_string(tabu_list_size) + "_" +
           "nbh" + to_string(neighbourhood_size) + "_" +
           "T0_" + doubleToString(initial_temperature) + "_" +
           "Tf_" + doubleToString(final_temperature) + "_" +
           "maxTh_" + doubleToString(max_heating_temperature) + "_" +
           "iterHeat_" + to_string(iteration_to_start_heating) + "_" +
           "coolScheme_" + cool_scheme_name + "_" +
           "heatScheme_" + heat_scheme_name;
}
