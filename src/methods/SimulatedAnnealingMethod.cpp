#include "Methods.h"
#include "Utils.h"

bool SimulatedAnnealingMethod::annealing(int currentScore) {
    return exp((best_individual.getFitness() - currentScore) / current_temperature) < real_dist(random_engine);
}

Individual *SimulatedAnnealingMethod::findBestIndividual(vector<Individual> &individuals) {
    Individual *bestInd = nullptr;

    for (auto &ind: individuals)
        if (bestInd == nullptr || *bestInd > ind || annealing(ind.getFitness()))
            bestInd = &ind;

    return bestInd;
}

void SimulatedAnnealingMethod::cooling() {
    if (current_temperature > final_temperature)
        current_temperature = std::max(cooling_scheme(current_temperature, cooling_ratio), final_temperature);
}

void SimulatedAnnealingMethod::algorithmStep(Individual &currentIndividual, vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentIndividual, mutation, neighbourhood_size);
    auto tempInd = findBestIndividual(neighborhood);

    if (!tempInd) return;
    currentIndividual = *tempInd;

    if (currentIndividual < best_individual) best_individual = currentIndividual;

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

    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    best_individual = currentIndividual;
    vector<Individual> neighborhood;

    Statistics statistics(iterations);

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentIndividual, neighborhood);
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
