#ifndef METHODS_H
#define METHODS_H

#include "Individual.h"
#include "Problem.h"
#include "Statistics.h"
#include "TabuList.h"
#include "unordered_map"
#include <memory>
#include <utility>

using namespace std;

class Method {
protected:
    Problem &problem;
    mt19937 random_engine;
    string short_name;
    int iterations;

    Individual best_individual;

    static vector<Individual> generateNeighbourhood(const Individual &individual, Mutation &mutation, int size) ;

private:
    virtual void algorithmStep(Individual &currentIndividual, vector<Individual> &neighborhood) = 0;

    virtual Individual& findBestIndividual(vector<Individual> &individuals, Individual &currentInd) = 0;

    virtual unique_ptr<Method> clone() const = 0;

    virtual void reset() = 0;

public:
    explicit Method(Problem &problem, int iterations, mt19937 randomEngine) :
            problem(problem),
            iterations(iterations),
            random_engine(randomEngine),
            best_individual(problem.createRandomIndividual(randomEngine)) {};

    virtual ~Method() = default;

    virtual Individual run() = 0;

    virtual Individual runAndSave() = 0;

    virtual string getFileName() const = 0;

    vector<Individual> runManyTimes(int numberOfRuns = 10);

    static void runEachMethodManyTimesAndSave(const Problem &problem, const vector<Method *> &methods, int numberOfRuns = 10);

    static void runEachMethodOnceAndSave(const vector<Method *> &methods);
};

class TabuSearchMethod : public Method {
private:
    Mutation &mutation;
    int tabu_list_size;
    int neighbourhood_size;

    // Values to reset before each run
    TabuList tabu_list;

    unique_ptr<Method> clone() const override { return make_unique<TabuSearchMethod>(*this); }

    void algorithmStep(Individual &currentIndividual, vector<Individual> &neighborhood) override;

    Individual& findBestIndividual(vector<Individual> &individuals, Individual &currentInd) override;

    void reset() override;

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
            tabu_list_size(tabuListSize),
            neighbourhood_size(neighbourhoodSize) {
        short_name = "TS";
    };

    Individual run() override;

    Individual runAndSave() override;

    string getFileName() const override;
};

class SimulatedAnnealingMethod : public Method {
private:
    Mutation &mutation;
    int neighbourhood_size;
    double initial_temperature;
    double final_temperature;
    double (*cooling_scheme)(double, double);
    double cooling_ratio;
    string cool_scheme_name;
    uniform_real_distribution<double> real_dist;

    // Values to reset before each run
    double current_temperature{};

    unique_ptr<Method> clone() const override { return make_unique<SimulatedAnnealingMethod>(*this); }

    void reset() override;

    void cooling();

    bool annealing(double newIndScore, double oldIndScore);

    void algorithmStep(Individual &currentBestIndividual, vector<Individual> &neighborhood) override;

    Individual& findBestIndividual(vector<Individual> &individuals, Individual &currentInd) override;

public:
    SimulatedAnnealingMethod(Problem &problem,
                             int iterations,
                             Mutation &mutation,
                             int neighbourhoodSize,
                             double initialTemperature,
                             double finalTemperature,
                             double coolingRatio,
                             double (*coolingScheme)(double, double),
                             string coolSchemeName,
                             mt19937 randomEngine) :
            Method(problem, iterations, randomEngine),
            mutation(mutation),
            initial_temperature(initialTemperature),
            final_temperature(finalTemperature),
            cooling_ratio{coolingRatio},
            cooling_scheme(coolingScheme),
            cool_scheme_name(std::move(coolSchemeName)),
            neighbourhood_size(neighbourhoodSize) {
        short_name = "SA";
    };

    Individual run() override;

    Individual runAndSave() override;

    string getFileName() const override;
};

class HybridTabuSAMethod : public Method {
private:
    Mutation &mutation;
    int iteration_to_start_heating;
    int neighbourhood_size;
    int tabu_list_size;
    double initial_temperature;
    double final_temperature;
    double max_heating_temperature;
    double (*cooling_scheme)(double, double);
    double cooling_ratio;
    double heating_ratio;
    double (*heating_scheme)(double, double);
    string cool_scheme_name;
    string heat_scheme_name;
    uniform_real_distribution<double> real_dist;

    // Values to reset before each run
    TabuList tabu_list;
    double current_temperature{};

    unique_ptr<Method> clone() const override { return make_unique<HybridTabuSAMethod>(*this); }

    void reset() override;

    bool isBest(Individual &ind, Individual *bestInd);

    bool annealing(double newIndScore, double oldIndScore);

    bool cooling();

    bool heating();

    void updateTemperature(bool &isCooling, int &iterToChange);

    void algorithmStep(Individual &currentBestIndividual, vector<Individual> &neighborhood) override;

    Individual& findBestIndividual(vector<Individual> &individuals, Individual &currentInd) override;

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
                       double (*coolingScheme)(double, double ),
                       double cooling_ratio,
                       double heating_ratio,
                       double (*heatingScheme)(double, double ),
                       string coolSchemeName,
                       string heatSchemeName,
                       mt19937 randomEngine) :
            Method(problem, iterations, randomEngine),
            iteration_to_start_heating(iterationsToStartHeating),
            mutation(mutation),
            tabu_list(tabuSize),
            tabu_list_size(tabuSize),
            neighbourhood_size(neighbourhoodSize),
            initial_temperature(initialTemperature),
            final_temperature(finalTemperature),
            max_heating_temperature(maxHeatingTemperature),
            cooling_scheme(coolingScheme),
            cooling_ratio(cooling_ratio),
            heating_scheme(heatingScheme),
            heating_ratio(heating_ratio),
            cool_scheme_name(std::move(coolSchemeName)),
            heat_scheme_name(std::move(heatSchemeName)) {
        short_name = "HTSA";
    };

    Individual run() override;

    Individual runAndSave() override;

    string getFileName() const override;
};

#endif //METHODS_H
