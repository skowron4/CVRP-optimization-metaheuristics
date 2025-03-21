#include "Methods.h"

//there is no mutation probability so the search may be less efficient (especially at the end of the algorithm)
vector<Individual> Method::generateNeighbourhood(const Individual &individual, Mutation &mutation, int size) const {
    vector<Individual> newNeighbourhood(size, individual);

    for(Individual ind : newNeighbourhood) ind.mutate(mutation);

    return std::move(newNeighbourhood);
}

Statistics Method::runManyTimes(int numberOfRuns) {
    for (int i = 0; i < numberOfRuns; ++i) {
        run();
    }
}
