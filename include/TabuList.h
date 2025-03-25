#ifndef CVRP_OPTIMIZATION_METAHEURISTICS_TABULIST_H
#define CVRP_OPTIMIZATION_METAHEURISTICS_TABULIST_H

#include "Individual.h"
#include <deque>
#include <unordered_set>

using namespace std;

class TabuList {
private:
    int tabu_list_size;
    deque<Individual> tabu_list;
    unordered_set<Individual, Individual::Hash, Individual::Equal> tabu_set;

public:
    explicit TabuList(int tabuListSize) : tabu_list_size(tabuListSize) {};

    TabuList(const TabuList &tabuList) : tabu_list_size(tabuList.tabu_list_size) {};

    void add(const Individual &individual);

    bool contains(const Individual &individual) const;

    void clear();
};

#endif //CVRP_OPTIMIZATION_METAHEURISTICS_TABULIST_H
