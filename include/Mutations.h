#ifndef MUTATIONS_H
#define MUTATIONS_H

#include <random>

using namespace std;

class Mutation {
protected:
    mt19937 random_engine;
    vector<int> indices;

    explicit Mutation(mt19937 &randomEngine) : random_engine(randomEngine) {};

    void updateIndices(size_t genotype_size);

public:
    virtual void mutate(vector<int> &genotype) = 0;

    virtual string getName() const = 0;
};

class SingleSwapMutation : public Mutation {
public:
    explicit SingleSwapMutation(mt19937 randomEngine) : Mutation(randomEngine) {};

    void mutate(vector<int> &genotype) override;

    string getName() const override { return "single-swap"; }
};

class InversionMutation : public Mutation {
public:
    explicit InversionMutation(mt19937 randomEngine) : Mutation(randomEngine) {};

    void mutate(vector<int> &genotype) override;

    string getName() const override { return "inversion"; }
};

#endif //MUTATIONS_H
