#include "TabuList.h"

void TabuList::add(const Individual &individual) {
    if (tabu_list.size() == tabu_list_size) {
        tabu_set.erase(tabu_list.front());
        tabu_list.pop_front();
    }
    tabu_list.push_back(individual);
    tabu_set.insert(individual);
}

bool TabuList::contains(const Individual &individual) const {
    return tabu_set.find(individual) != tabu_set.end();
}

void TabuList::clear() {
    tabu_list.clear();
    tabu_set.clear();
}
