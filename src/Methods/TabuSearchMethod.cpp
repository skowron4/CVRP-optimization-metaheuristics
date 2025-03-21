#include <algorithm>
#include "Methods.h"

void TabuSearchMethod::algorithmStep(Individual &currentIndividual,
                                     Individual &bestIndividual,
                                     vector<Individual> &neighborhood) {

    neighborhood = generateNeighbourhood(currentIndividual, mutation, neighbourhood_size);
    currentIndividual = *min_element(neighborhood.begin(), neighborhood.end());
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

    statistics.saveToFile("placeholderfile");

    return bestIndividual;
}