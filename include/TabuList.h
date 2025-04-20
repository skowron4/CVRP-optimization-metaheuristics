#ifndef CVRP_OPTIMIZATION_METAHEURISTICS_TABULIST_H
#define CVRP_OPTIMIZATION_METAHEURISTICS_TABULIST_H

#include "Individual.h"
#include <deque>
#include <unordered_set>

using namespace std;

class TabuList {
    int tabu_list_size;
    deque<Individual> tabu_list;
    unordered_set<Individual, Individual::Hash, Individual::Equal> tabu_set;

public:
    explicit TabuList(int tabuListSize) : tabu_list_size(tabuListSize) {};

    void add(const Individual &individual);

    bool contains(const Individual &individual) const;
};

#endif //CVRP_OPTIMIZATION_METAHEURISTICS_TABULIST_H
