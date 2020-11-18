#include "qlearning.h"


using namespace std;

template<typename Iter, typename RandomGenerator>
Iter select_randomly_rd(Iter start, Iter end, RandomGenerator& g) {
    uniform_int_distribution<> dis(0, distance(start, end) - 1);
    advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static random_device rd;
    static mt19937 gen(rd());
    return select_randomly_rd(start, end, gen);
}


Qlearning::Qlearning()
{

}


// Initialize the q-table and the environment
void Qlearning::initQTable(){

    vector<vector<float> > q_table_init(numberOfStates,std::vector<float>(numberOfStates, 0.0));


    q_table = q_table_init;

//    for(int i = 0; i < numberOfStates; i++){
//        for(int j = 0; j < numberOfStates; j++){
//            std::cout << q_table[i][j] << " ";
//        }
//       std::cout << std::endl;
//    }

}


void Qlearning::initRewardMatrix(){

//    vector<int> room1 = {-1, 20, -1, -1, -1, -1, -1, -1, -1, -1, -1};
//    vector<int> room2 = {-1, -1, 5, -1, -1, 0, -1, -1, -1, -1, -1};
//    vector<int> room3 = {-1, 20, -1, 5, 15, 0, -1, -1, -1, -1, -1};
//    vector<int> room4 = {-1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1};
//    vector<int> room5 = {-1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1};
//    vector<int> room6 = {-1, 20, 5, -1, -1, -1, 15, 5, 5, 5, -1};
//    vector<int> room7 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
//    vector<int> room8 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
//    vector<int> room9 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
//    vector<int> room10 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 20};
//    vector<int> room11 = {-1, -1, -1, -1, -1, -1, -1, -1, -1, 5, -1};

    vector<int> room1 = {-1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    vector<int> room2 = {-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1};
    vector<int> room3 = {-1, 0, -1, 0, 0, 0, -1, -1, -1, -1, -1};
    vector<int> room4 = {-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1};
    vector<int> room5 = {-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1};
    vector<int> room6 = {-1, 0, 0, -1, -1, -1, 0, 0, 0, 0, -1};
    vector<int> room7 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
    vector<int> room8 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
    vector<int> room9 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
    vector<int> room10 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 500};
    vector<int> room11 = {-1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1};

    reward_matrix.push_back(room1);
    reward_matrix.push_back(room2);
    reward_matrix.push_back(room3);
    reward_matrix.push_back(room4);
    reward_matrix.push_back(room5);
    reward_matrix.push_back(room6);
    reward_matrix.push_back(room7);
    reward_matrix.push_back(room8);
    reward_matrix.push_back(room9);
    reward_matrix.push_back(room10);
    reward_matrix.push_back(room11);

//    for(int i = 0; i < numberOfStates; i++){
//        for(int j = 0; j < numberOfStates; j++){
//            std::cout << reward_matrix[i][j] << " ";
//        }
//       std::cout << std::endl;
//    }

}



// Get random init state
void Qlearning::setRandomInitState(){

    random_device rd;
    uniform_int_distribution<int> random(0, numberOfStates);

    initial_state = random(rd);
}


// Get an action from the given state
int Qlearning::getAction(int current_state){

    vector<int> valid_actions = {};

    for(int action = 0; action < reward_matrix[current_state].size(); action++){
        if(reward_matrix[current_state][action] != -1){
            valid_actions.push_back(action);
        }
    }

    //return *select_randomly(valid_actions.begin(), valid_actions.end()); // could be a random one of valid actions

    random_device random_device;
    mt19937 engine{random_device()};
    uniform_int_distribution<int> dist(0, valid_actions.size() - 1);

    return valid_actions[dist(engine)];
}



// Get and take an action
int Qlearning::takeAction(int current_state, bool display){

    int action = getAction(current_state);
    float current_sa_reward = reward_matrix[current_state][action];
    auto max = max_element(q_table[action].begin(), q_table[action].end());
    float future_sa_reward = *max;
    float Q_value_current_state = current_sa_reward + gamma * future_sa_reward;
    q_table[current_state][action] = Q_value_current_state;
    int new_state = action;

    if(display){
        for (int state = 0; state < q_table[action].size(); state++) {
            for (int action = 0; action < q_table[action].size(); action++) {
                std::cout << q_table[state][action] << " ";
            }
            cout << endl;
        }
        cout << "Old state: " << current_state << " | " << "New state: " << new_state << endl;
    }

    return new_state;

}



// Run one episode
void Qlearning::runEpisode(bool display){
    int current_state = initial_state_random;
    while(true){
         current_state = takeAction(current_state, display);
         if(current_state == goal_state){
             cout << "Goal Reached!" << endl;
             break;
         }
    }
}



// Train the agent - Run several episodes
void Qlearning::train(bool display){

    cout << "Training started..." << endl;
    for (int episode = 0; episode < episodes; episode++) {
        setRandomInitState();
        runEpisode(display);
        cout << "Episode: " << episode << endl;
    }

    cout << "Training done." << endl;

}



void Qlearning::displayTrainedQTable(){

    for(int state = 0; state < numberOfStates; state++){
        for(int action = 0; action < numberOfStates; action++){
            std::cout << q_table[state][action] << " ";
        }
       std::cout << std::endl;
    }
}



// Deploy the trained agent
void Qlearning::deployAgent(int goal){

    goal_state = goal;
    setRandomInitState();
    int state = initial_state;
    int action;
    cout << "Start state: " << initial_state  << endl;
    int steps = 0;

    while(true){
        steps++;
        action = std::distance(q_table[state].begin(), max_element(q_table[state].begin(), q_table[state].end()));
        cout << "Actions taken: " << action<< endl;
        state = action;
        if(state == goal_state){
            cout << "Goal Reached!" << endl;
            cout << "Number of Steps: " << steps << endl;
            stepsToGoal = steps;
            break;
        }
    }
}
