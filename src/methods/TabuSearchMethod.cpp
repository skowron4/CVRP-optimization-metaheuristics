#include "Methods.h"
#include "Statistics.h"

Individual& TabuSearchMethod::findBestIndividual(vector<Individual> &individuals, Individual &currentInd) {
    Individual *best = nullptr;

    for (Individual &ind : individuals)
        if (!tabu_list.contains(ind) && (best == nullptr || *best > ind)) best = &ind;

    return best ? *best : currentInd;
}

void TabuSearchMethod::algorithmStep(Individual &currentIndividual, vector<Individual> &neighborhood) {
    neighborhood = generateNeighbourhood(currentIndividual, neighbourhood_size);
    currentIndividual = findBestIndividual(neighborhood, currentIndividual);

    if (currentIndividual < best_individual) {
        best_individual = currentIndividual;
        tabu_list.add(currentIndividual);
    }
}

Individual TabuSearchMethod::run() {
    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    best_individual = currentIndividual;
    vector<Individual> neighborhood;

    tabu_list.add(currentIndividual);

    for (int i = 0; i < iterations; ++i) algorithmStep(currentIndividual, neighborhood);

    return best_individual;
}

Individual TabuSearchMethod::runAndSave(const path &outDir) {
    Individual currentIndividual = problem.createRandomIndividual(random_engine);
    best_individual = currentIndividual;
    vector<Individual> neighborhood;

    tabu_list.add(currentIndividual);

    Statistics statistics(iterations);

    for (int i = 0; i < iterations; ++i) {
        algorithmStep(currentIndividual, neighborhood);
        statistics.calculateAndAddStatisticsFitnessRecord(neighborhood);
    }

    statistics.saveToFile(getFileName(), outDir);

    return best_individual;
}

string TabuSearchMethod::getFileName() const {
    return problem.getName() + "_" + type + "_" +
           "mut_" + mutation->getType() + "_" +
           "iter_" + to_string(iterations) + "_" +
           "tabu_" + to_string(tabu_list_size) + "_" +
           "nbh" + to_string(neighbourhood_size);
}
