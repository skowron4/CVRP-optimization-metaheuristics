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
    string type;

    Problem &problem;
    int iterations;
    unique_ptr<Mutation> mutation;

    mt19937 random_engine;
    Individual best_individual;

    vector<Individual> generateNeighbourhood(const Individual &individual, int size);

private:
    virtual void algorithmStep(Individual &currentIndividual, vector<Individual> &neighborhood) = 0;

    virtual Individual &findBestIndividual(vector<Individual> &individuals, Individual &currentInd) = 0;

    static vector<vector<Individual>> runEachMethodBoxPlot(const vector<vector<unique_ptr<Method>>> &methods);

public:
    explicit Method(Problem &problem, int iterations, unique_ptr<Mutation> mutation) :
            problem(problem),
            iterations(iterations),
            mutation(std::move(mutation)),
            random_engine(chrono::system_clock::now().time_since_epoch().count()),
            best_individual(problem.createRandomIndividual(random_engine)) {};

    virtual ~Method() = default;

    virtual Individual run() = 0;

    virtual Individual runAndSave(const path &outDir) = 0;

    virtual string getFileName() const = 0;

    static void runEachMethodAndSaveBoxPlot(const Problem &problem, const vector<vector<unique_ptr<Method>>> &methods, const path &outDir);

    static void runEachMethodAndSaveSinglePlots(const vector<unique_ptr<Method>> &methods, const path &outDir);
};

class TabuSearchMethod : public Method {
private:
    int tabu_list_size;
    int neighbourhood_size;

    TabuList tabu_list;

    void algorithmStep(Individual &currentIndividual, vector<Individual> &neighborhood) override;

    Individual &findBestIndividual(vector<Individual> &individuals, Individual &currentInd) override;

public:
    TabuSearchMethod(Problem &problem,
                     int iterations,
                     unique_ptr<Mutation> mutation,
                     int tabuListSize,
                     int neighbourhoodSize) :
            Method(problem, iterations, std::move(mutation)),
            tabu_list(tabuListSize),
            tabu_list_size(tabuListSize),
            neighbourhood_size(neighbourhoodSize) { type = "TS"; };

    Individual run() override;

    Individual runAndSave(const path &outDir) override;

    string getFileName() const override;
};

class SimulatedAnnealingMethod : public Method {
private:
    int neighbourhood_size;
    double initial_temperature;
    double final_temperature;

    double cooling_ratio;
    double (*cooling_scheme)(double, double);
    string cool_scheme_name;

    uniform_real_distribution<double> real_dist;

    double current_temperature;

    void cooling();

    bool annealing(double newIndScore, double oldIndScore);

    void algorithmStep(Individual &currentBestIndividual, vector<Individual> &neighborhood) override;

    Individual &findBestIndividual(vector<Individual> &individuals, Individual &currentInd) override;

public:
    SimulatedAnnealingMethod(Problem &problem,
                             int iterations,
                             unique_ptr<Mutation> mutation,
                             int neighbourhoodSize,
                             double initialTemperature,
                             double finalTemperature,
                             double coolingRatio,
                             double (*coolingScheme)(double, double),
                             string coolSchemeName) :
            Method(problem, iterations, std::move(mutation)),
            neighbourhood_size(neighbourhoodSize),
            initial_temperature(initialTemperature),
            current_temperature(initialTemperature),
            final_temperature(finalTemperature),
            cooling_ratio(coolingRatio),
            cooling_scheme(coolingScheme),
            cool_scheme_name(std::move(coolSchemeName)) { type = "SA"; };

    Individual run() override;

    Individual runAndSave(const path &outDir) override;

    string getFileName() const override;
};

class HybridTabuSAMethod : public Method {
private:
    int iteration_to_start_heating;
    int tabu_list_size;
    int neighbourhood_size;
    double initial_temperature;
    double final_temperature;
    double max_heating_temperature;

    double cooling_ratio;
    double (*cooling_scheme)(double, double);
    string cool_scheme_name;

    double heating_ratio;
    double (*heating_scheme)(double, double);
    string heat_scheme_name;

    uniform_real_distribution<double> real_dist;

    TabuList tabu_list;
    double current_temperature;

    bool annealing(double newIndScore, double oldIndScore);

    bool cooling();

    bool heating();

    void updateTemperature(bool &isCooling, int &iterToChange);

    void algorithmStep(Individual &currentBestIndividual, vector<Individual> &neighborhood) override;

    Individual &findBestIndividual(vector<Individual> &individuals, Individual &currentInd) override;

public:
    HybridTabuSAMethod(Problem &problem,
                       int iterations,
                       unique_ptr<Mutation> mutation,
                       int iterationsToStartHeating,
                       int tabuSize,
                       int neighbourhoodSize,
                       double initialTemperature,
                       double finalTemperature,
                       double maxHeatingTemperature,
                       double coolingRatio,
                       double (*coolingScheme)(double, double),
                       string coolSchemeName,
                       double heatingRatio,
                       double (*heatingScheme)(double, double),
                       string heatSchemeName) :
            Method(problem, iterations, std::move(mutation)),
            iteration_to_start_heating(iterationsToStartHeating),
            tabu_list(tabuSize),
            tabu_list_size(tabuSize),
            neighbourhood_size(neighbourhoodSize),
            initial_temperature(initialTemperature),
            current_temperature(initialTemperature),
            final_temperature(finalTemperature),
            max_heating_temperature(maxHeatingTemperature),
            cooling_ratio(coolingRatio),
            cooling_scheme(coolingScheme),
            cool_scheme_name(std::move(coolSchemeName)),
            heating_ratio(heatingRatio),
            heating_scheme(heatingScheme),
            heat_scheme_name(std::move(heatSchemeName)) { type = "HTSA"; };

    Individual run() override;

    Individual runAndSave(const path &outDir) override;

    string getFileName() const override;
};

#endif //METHODS_H
