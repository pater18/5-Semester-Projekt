#ifndef QLEARNING_H
#define QLEARNING_H

#include <iostream>
#include <vector>

using namespace std;

class Qlearning
{
public:
    Qlearning();
    Qlearning(int initState)
        : initial_state(initState) {}

    // Initialize the q-table and the environment
    void init_qTable();
    void init_reward_matrix();

    // Get random init state
    void get_random_init_state();

    // Get and take an action
    int take_action();
    int get_action();

    // Run one episode
    void episode();

    // Train the agent - Run several episodes
    void train();



private:

    vector<vector<float> > q_table;
    vector<vector<int> > reward_matrix;

    int numberOfStates = 11;
    int initial_state = 0;
    int initial_state_random = 0;

    float gamma = 0.9;
    float learning_rate = 1.0;
    float learning_rate_decay = 0.001;
    float epsilon = 1.0;
    float epsilon_decay = 0.001;
    int episodes = 1000;
    int maxStepsPerEpisode = 100;

};

#endif // QLEARNING_H
