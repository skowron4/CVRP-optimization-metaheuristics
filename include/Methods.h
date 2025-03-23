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
    virtual void algorithmStep(Individual &currentIndividual,
                               Individual &bestIndividual,
                               vector<Individual> &neighborhood) = 0;
    virtual Individual* findBestIndividual(vector<Individual> &neighborhood) = 0;

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

protected:
    void algorithmStep(Individual &currentIndividual,
                       Individual &bestIndividual,
                       vector<Individual> &neighborhood) override;
    Individual* findBestIndividual(vector<Individual> &neighborhood) override;

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

    Individual run() override;

    Individual runAndSave() override;
};

class SimulatedAnnealingMethod : public Method {
private:
    Mutation &mutation;
    int neighbourhood_size;
    double initial_temperature;
    double current_temperature;
    double final_temperature;
    double (*cooling_scheme)(double);
    int best_score;
    std::uniform_real_distribution<double> real_dist;

    void cooling();

    bool annealing(int currentScore);

protected:
    void algorithmStep(Individual &currentIndividual,
                       Individual &bestIndividual,
                       vector<Individual> &neighborhood) override;

    Individual* findBestIndividual(vector<Individual> &neighborhood) override;

public:
    SimulatedAnnealingMethod(Problem &problem,
                             int iterations,
                             Mutation &mutation,
                             int neighbourhoodSize,
                             double initialTemperature,
                             double finalTemperature,
                             double (*coolingScheme)(double),
                             mt19937 randomEngine) :
            Method(problem, iterations, randomEngine),
            mutation(mutation),
            initial_temperature(initialTemperature),
            current_temperature(0),
            final_temperature(finalTemperature),
            cooling_scheme(coolingScheme),
            neighbourhood_size(neighbourhoodSize),
            best_score(0) {};

    Method *clone() const override { return new SimulatedAnnealingMethod(*this); }

    Individual run() override;

    Individual runAndSave() override;
};

class HybridTabuSAMethod : public Method {
private:
    Mutation &mutation;
    int iteration_to_start_heating;
    int neighbourhood_size;
    TabuList tabu_list;
    double initial_temperature;
    double current_temperature;
    double final_temperature;
    double max_heating_temperature;
    double (*cooling_scheme)(double);
    double (*heating_scheme)(double);
    std::uniform_real_distribution<double> real_dist;
    int best_score;

    bool isBest(Individual &ind, Individual *bestInd);

    bool annealing(int current);

    bool cooling();

    bool heating();

    void updateTemperature(bool &isCooling, int &iterToChange);

protected:
    void algorithmStep(Individual &currentIndividual,
                       Individual &bestIndividual,
                       vector<Individual> &neighborhood) override;

    Individual* findBestIndividual(vector<Individual> &neighborhood) override;


public:
    HybridTabuSAMethod(Problem &problem,
                       int iterations,
                       int iterationsToStartHeating,
                       Mutation &mutation,
                       int tabuSize,
                       int neighbourhoodSize,
                       double initialTemperature,
                       double finalTemperature,
                       double maxHeatingTemperature,
                       double (*coolingScheme)(double),
                       double (*heatingScheme)(double),
                       mt19937 randomEngine) :
            Method(problem, iterations, randomEngine),
            iteration_to_start_heating(iterationsToStartHeating),
            mutation(mutation),
            tabu_list(tabuSize),
            neighbourhood_size(neighbourhoodSize),
            initial_temperature(initialTemperature),
            current_temperature(initialTemperature),
            final_temperature(finalTemperature),
            max_heating_temperature(maxHeatingTemperature),
            cooling_scheme(coolingScheme),
            heating_scheme(heatingScheme),
            best_score(0) {};

    Method *clone() const override { return new HybridTabuSAMethod(*this); }

    Individual run() override;

    Individual runAndSave() override;

};

#endif //METHODS_H
