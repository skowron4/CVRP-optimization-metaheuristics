#include "Methods.h"
#include "Utils.h"

bool SimulatedAnnealingMethod::annealing(double newIndScore, double oldIndScore) {
    return exp((oldIndScore - newIndScore) / current_temperature) > real_dist(random_engine);
}

Individual& SimulatedAnnealingMethod::findBestIndividual(vector<Individual> &individuals, Individual &currentInd) {
    for (auto &ind: individuals)
        if (currentInd > ind || annealing(ind.getFitness(), currentInd.getFitness()))
            currentInd = ind;
    return currentInd;
}

void SimulatedAnnealingMethod::cooling() {
    if (current_temperature > final_temperature)
        current_temperature = max(cooling_scheme(current_temperature, cooling_ratio), final_temperature);
}

void SimulatedAnnealingMethod::algorithmStep(Individual &currentBestIndividual, vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentBestIndividual, mutation, neighbourhood_size);
    currentBestIndividual = findBestIndividual(neighborhood, currentBestIndividual);

    if (currentBestIndividual < best_individual) best_individual = currentBestIndividual;

    cooling();
}

Individual SimulatedAnnealingMethod::run() {
    reset();

    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    best_individual = currentIndividual;
    vector<Individual> neighborhood;

    for (int i = 0; i < iterations; ++i) algorithmStep(currentIndividual, neighborhood);

    return best_individual;
}

Individual SimulatedAnnealingMethod::runAndSave() {
    reset();

    Individual currentBestIndividual = problem.createRandomIndividual(random_engine);
    best_individual = currentBestIndividual;
    vector<Individual> neighborhood;

    Statistics statistics(iterations);

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentBestIndividual, neighborhood);
        statistics.calculateAndAddStatisticsFitnessRecord(neighborhood);
    }

    statistics.saveToFile(getFileName());

    return best_individual;
}

string SimulatedAnnealingMethod::getFileName() const {
    return problem.getName() + "_" + short_name + "_" +
           "mut_" + mutation.getName() + "_" +
           "iter_" + to_string(iterations) + "_" +
           "neigh_" + to_string(neighbourhood_size) + "_" +
           "T0_" + doubleToString(initial_temperature) + "_" +
           "Tf_" + doubleToString(final_temperature) + "_" +
           "coolScheme_" + cool_scheme_name + "_" +
           "coolRatio_" + to_string(cooling_ratio);
}

void SimulatedAnnealingMethod::reset() {
    current_temperature = initial_temperature;
}
