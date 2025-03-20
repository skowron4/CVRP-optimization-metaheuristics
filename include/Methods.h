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

private:
    const Problem &problem;

    vector<Individual> generateNeighbourhood(const Individual &individual, Mutation &mutation, int size) const;

public:
    explicit Method(const Problem &problem, int iterations) :
            problem(problem),
            iterations(iterations) {};

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
    TabuSearchMethod(const Problem &problem, int iterations, Mutation &mutation, int tabuListSize,
                     int neighbourhoodSize) :
            Method(problem, iterations),
            mutation(mutation),
            tabu_list(tabuListSize),
            neighbourhood_size(neighbourhoodSize) {};

    Method *clone() const override { return new TabuSearchMethod(*this); }

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
    SimulatedAnnealingMethod(const Problem &problem, int iterations, Mutation &mutation, int neighbourhoodSize,
                             double initialTemperature, double finalTemperature, double coolingRate) :
            Method(problem, iterations),
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
    HybridTabuSAMethod(const Problem &problem, int iterations, Mutation &tabuMutation, Mutation &saMutation,
                       int tabuListSize, int tabuNeighbourhoodSize,
                       int saNeighbourhoodSize, double initialTemperature, double finalTemperature, double coolingRate)
            :
            Method(problem, iterations),
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
