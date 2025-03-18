#ifndef MUTATIONS_H
#define MUTATIONS_H

#include <random>

using namespace std;

class Mutation {
protected:
    mt19937 random_engine;
    std::uniform_int_distribution<int> int_dist1, int_dist2;
    vector<int> indices;

    explicit Mutation(mt19937 &randomEngine) : random_engine(randomEngine) {};

    void updateIndices(size_t genotype_size) {
        if (indices.size() != genotype_size) {
            indices.resize(genotype_size);
            iota(indices.begin(), indices.end(), 0);
        }
    }

public:
    virtual void mutate(vector<int> &genotype);

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
