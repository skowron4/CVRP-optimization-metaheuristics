#include "Problem.h"
#include "Individual.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

void Problem::computeDistanceMatrix() {
    distance_matrix.resize(data.dimension, vector<int>(data.dimension, 0));
    for (int i = 0; i < data.dimension; i++) {
        for (int j = i + 1; j < data.dimension; j++) {
            int x1{static_cast<int>(data.cities[i].x)},
            y1{static_cast<int>(data.cities[i].y)},
            x2{static_cast<int>(data.cities[j].x)},
            y2{static_cast<int>(data.cities[j].y)};

            int distance = static_cast<int>(round(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))));
            distance_matrix[i][j] = distance;
            distance_matrix[j][i] = distance;
        }
    }
}

Individual Problem::createGreedyIndividual(int startingCity) {
    vector<int> cities;
    cities.reserve(cities_count);
    for (int j = 1; j <= data.dimension; ++j) {
        if (j == data.depot || j == startingCity) continue;
        cities.push_back(j);
    }

    vector<int> genotype(genotype_size, 0);
    genotype[0] = startingCity;

    int i{1}, curr{startingCity}, currLoad{data.capacity - data.demands[startingCity - 1].value};
    while (!cities.empty()) {
        double minDist{numeric_limits<double>::max()};
        int next{data.depot};

        for (int city: cities) {
            if (currLoad - data.demands[city - 1].value < 0) continue;
            int dist{distance_matrix[curr - 1][city - 1]};
            if (dist < minDist) {
                minDist = dist;
                next = city;
            }
        }
        curr = next;

        if (next == data.depot) currLoad = data.capacity;
        else {
            genotype[i] = next;
            currLoad -= data.demands[next - 1].value;
            cities.erase(remove(cities.begin(), cities.end(), next), cities.end());
        }

        i++;
    }

    return {*this, genotype};
}

int Problem::evaluateGenotype(const vector<int> &genotype) const {
    int curr{data.depot}, currLoad{data.capacity};
    double dist{0.0};

    for (int next: genotype) {
        if (next == 0) {
            dist += distance_matrix[curr - 1][data.depot - 1];
            currLoad = data.capacity;
            curr = data.depot;
        } else {
            currLoad -= data.demands[next - 1].value;
            if (currLoad < 0) {
                dist += distance_matrix[curr - 1][data.depot - 1] + distance_matrix[data.depot - 1][next - 1];
                currLoad = data.capacity - data.demands[next - 1].value;
            } else dist += distance_matrix[curr - 1][next - 1];
            curr = next;
        }
    }
    return dist + distance_matrix[curr - 1][data.depot - 1];
}

Individual Problem::createRandomIndividual(mt19937 &random_engine) {
    vector<int> genotype(genotype_size, 0);

    for (int i = 1; i < data.depot; ++i) genotype[i] = i;
    for (int i = data.depot + 1; i <= data.dimension; ++i) genotype[i] = i;

    shuffle(genotype.begin(), genotype.end(), random_engine);
    return {*this, genotype};
}

vector<Individual> Problem::createGreedyIndividuals() {
    vector<Individual> individuals;
    individuals.reserve(cities_count);
    for (int i = 1; i < data.depot; ++i) individuals.push_back(createGreedyIndividual(i));
    for (int i = data.depot + 1; i <= data.dimension; ++i) individuals.push_back(createGreedyIndividual(i));
    return individuals;
}

void Problem::printData() const {
    cout << "Name: " << data.name << endl;
    cout << "Comment: " << data.comment << endl;
    cout << "Type: " << data.type << endl;
    cout << "Dimension: " << data.dimension << endl;
    cout << "Edge weight type: " << data.edge_weight_type << endl;
    cout << "Capacity: " << data.capacity << endl;
    cout << "Depot: " << data.depot << endl;

    cout << "Demands:" << endl;
    for (auto &demand: data.demands)
        cout << "City: " << demand.city_id << " Demand: " << demand.value << endl;
}

void Problem::printDistanceMatrix() const {
    cout << "Distance matrix:" << endl;
    int cellWidth{4};

    // Print column headers
    cout << setw(cellWidth) << "\\";
    for (int i = 0; i < data.dimension; ++i) cout << setw(cellWidth) << i + 1;

    // Print rows
    for (int i = 0; i < data.dimension; ++i) {
        cout << endl << setw(cellWidth) << i + 1;
        for (int j = 0; j < data.dimension; ++j) cout << setw(cellWidth) << distance_matrix[i][j];
    }

    cout << endl << endl;
}
