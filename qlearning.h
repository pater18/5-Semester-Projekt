#ifndef QLEARNING_H
#define QLEARNING_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

class Qlearning
{
public:
    Qlearning();
    Qlearning(int episodes)
        : episodes(episodes) {}

    // Initialize the q-table and the environment
    void initQTable();
    void initRewardMatrix();

    // Get random init state
    void setRandomInitState();

    // Get and take an action
    int takeAction(int current_state, bool display);
    int getAction(int current_state);

    // Run one episode
    void runEpisode(bool display);

    // Train the agent - Run several episodes
    void train(bool display);

    void displayTrainedQTable();

    // Deploy the trained agent
    void deployAgent(int goal);

    void setGoalState(int goal){ goal_state = goal; }

    int getStepsToGoal(){ return stepsToGoal; }

    void normalizeQTable();

private:

    vector<vector<float> > q_table;
    vector<vector<int> > reward_matrix;

    int goal_state = 10;
    int stepsToGoal = -1;

    int numberOfStates = 11;
    int initial_state;
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
