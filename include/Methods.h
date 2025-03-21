#ifndef METHODS_H
#define METHODS_H

#include "Individual.h"
#include "Problem.h"
#include "Statistics.h"
#include "TabuList.h"

using namespace std;

class Method {
protected:
    int iterations;
    Problem &problem;
    mt19937 random_engine;
    vector<Individual> generateNeighbourhood(const Individual &individual, Mutation &mutation, int size) const;

public:
    explicit Method(Problem &problem, int iterations, mt19937 randomEngine) :
            problem(problem),
            iterations(iterations),
            random_engine(randomEngine) {};

    virtual Method *clone() const = 0;

    Statistics runManyTimes(int numberOfRuns = 10);

    virtual Individual run() = 0;

    virtual Individual runAndSave() = 0;
};

class TabuSearchMethod : public Method {
private:
    Mutation &mutation;
    TabuList tabu_list;
    int neighbourhood_size;

public:
    TabuSearchMethod(Problem &problem,
                     int iterations,
                     Mutation &mutation,
                     int tabuListSize,
                     int neighbourhoodSize,
                     mt19937 randomEngine) :
            Method(problem, iterations, randomEngine),
            mutation(mutation),
            tabu_list(tabuListSize),
            neighbourhood_size(neighbourhoodSize) {};

    Method *clone() const override { return new TabuSearchMethod(*this); }

    void algorithmStep(Individual &currentIndividual, Individual &bestIndividual, vector<Individual> &neighborhood);

    Individual run() override;

    Individual runAndSave() override;
};

class SimulatedAnnealingMethod : public Method {
private:
    Mutation &mutation;
    int neighbourhood_size;
    double initial_temperature;
    double final_temperature;
    double cooling_rate;

public:
    SimulatedAnnealingMethod(Problem &problem,
                             int iterations,
                             Mutation &mutation,
                             int neighbourhoodSize,
                             double initialTemperature,
                             double finalTemperature,
                             double coolingRate,
                             mt19937 randomEngine) :
            Method(problem, iterations, randomEngine),
            mutation(mutation),
            initial_temperature(initialTemperature),
            final_temperature(finalTemperature),
            cooling_rate(coolingRate),
            neighbourhood_size(neighbourhoodSize) {};

    Method *clone() const override { return new SimulatedAnnealingMethod(*this); }

    Individual run() override;

    Individual runAndSave() override;
};

class HybridTabuSAMethod : public Method {
private:
    Mutation &tabu_mutation;
    Mutation &sa_mutation;
    int tabu_neighbourhood_size;
    TabuList tabu_list;
    int sa_neighbourhood_size;
    double initial_temperature;
    double final_temperature;
    double cooling_rate;
public:
    HybridTabuSAMethod(Problem &problem,
                       int iterations,
                       Mutation &tabuMutation,
                       Mutation &saMutation,
                       int tabuListSize,
                       int tabuNeighbourhoodSize,
                       int saNeighbourhoodSize,
                       double initialTemperature,
                       double finalTemperature,
                       double coolingRate,
                       mt19937 randomEngine) :
            Method(problem, iterations, randomEngine),
            tabu_mutation(tabuMutation),
            sa_mutation(saMutation),
            tabu_list(tabuListSize),
            tabu_neighbourhood_size(tabuNeighbourhoodSize),
            sa_neighbourhood_size(saNeighbourhoodSize),
            initial_temperature(initialTemperature),
            final_temperature(finalTemperature),
            cooling_rate(coolingRate) {};

    Method *clone() const override { return new HybridTabuSAMethod(*this); }

    Individual run() override;

    Individual runAndSave() override;

};

#endif //METHODS_H
