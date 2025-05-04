#ifndef MUTATIONS_H
#define MUTATIONS_H

#include <random>
#include <chrono>

using namespace std;

class Mutation {
protected:
    string type;
    mt19937 random_engine;
    uniform_int_distribution<int> int_dist;

    explicit Mutation(int genotype_size)
            : random_engine(chrono::system_clock::now().time_since_epoch().count()),
              int_dist(0, genotype_size - 1) {}

public:
    virtual ~Mutation() = default;

    virtual void mutate(vector<int> &genotype) = 0;

    string getType() const { return type; }
};

class SingleSwapMutation : public Mutation {
public:
    explicit SingleSwapMutation(int genotype_size) : Mutation(genotype_size) { type = "single-swap"; }

    void mutate(vector<int> &genotype) override;
};

class InversionMutation : public Mutation {
public:
    explicit InversionMutation(int genotype_size) : Mutation(genotype_size) { type = "inversion"; }

    void mutate(vector<int> &genotype) override;
};

#endif //MUTATIONS_H
