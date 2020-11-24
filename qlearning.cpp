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

    // State init with distributed marbles in rooms - 1 marble is 5 points
    Room room0Reward(5);
    rooms.push_back((room0Reward));
    Room room1Reward(15);
    rooms.push_back(room1Reward);
    Room room2Reward(5);
    rooms.push_back(room2Reward);
    Room room3Reward(5);
    rooms.push_back(room3Reward);
    Room room4Reward(10);
    rooms.push_back(room4Reward);
    Room room5Reward(0);
    rooms.push_back(room5Reward);
    Room room6Reward(10);
    rooms.push_back(room6Reward);
    Room room7Reward(5);
    rooms.push_back(room7Reward);
    Room room8Reward(5);
    rooms.push_back(room8Reward);
    Room room9Reward(0);
    rooms.push_back(room9Reward);
    Room room10Reward(25);
    rooms.push_back(room10Reward);

    // Markov property - keeping track of which rooms have been visited
    for(int i = 0; i < numberOfStates; i++){
        rooms[i].roomNumber = i;
        rooms[i].isVisited = false;
    }


    // Reward matrix
    vector<int> room0 = {-1, room1Reward.reward, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    vector<int> room1 = {room0Reward.reward, -1, room2Reward.reward, -1, -1, room5Reward.reward, -1, -1, -1, -1, -1};
    vector<int> room2 = {-1, room1Reward.reward, -1, room3Reward.reward, room4Reward.reward, room5Reward.reward, -1, -1, -1, -1, -1};
    vector<int> room3 = {-1, -1, room2Reward.reward, -1, -1, -1, -1, -1, -1, -1, -1};
    vector<int> room4 = {-1, -1, room2Reward.reward, -1, -1, -1, -1, -1, -1, -1, -1};
    vector<int> room5 = {-1, room1Reward.reward, room2Reward.reward, -1, -1, -1, room6Reward.reward, room7Reward.reward, room8Reward.reward, room9Reward.reward, -1};
    vector<int> room6 = {-1, -1, -1, -1, -1, room5Reward.reward, -1, -1, -1, -1, -1};
    vector<int> room7 = {-1, -1, -1, -1, -1, room5Reward.reward, -1, -1, -1, -1, -1};
    vector<int> room8 = {-1, -1, -1, -1, -1, room5Reward.reward, -1, -1, -1, -1, -1};
    vector<int> room9 = {-1, -1, -1, -1, -1, room5Reward.reward, -1, -1, -1, -1, room10Reward.reward};
    vector<int> room10 = {-1, -1, -1, -1, -1, -1, -1, -1, -1, room9Reward.reward, -1};

//    vector<int> room0 = {-1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1};
//    vector<int> room1 = {-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1};
//    vector<int> room2 = {-1, 0, -1, 0, 0, 0, -1, -1, -1, -1, -1};
//    vector<int> room3 = {-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1};
//    vector<int> room4 = {-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1};
//    vector<int> room5 = {-1, 0, 0, -1, -1, -1, 0, 0, 0, 0, -1};
//    vector<int> room6 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
//    vector<int> room7 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
//    vector<int> room8 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
//    vector<int> room9 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 500};
//    vector<int> room10 = {-1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1};

    reward_matrix.push_back(room0);
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

    return *select_randomly(valid_actions.begin(), valid_actions.end()); // could be a random one of valid actions

//    std::random_device rd;
//    uniform_int_distribution<int> dist(0, valid_actions.size() - 1);

//    return valid_actions[dist(rd)];
}



// Get and take an action
int Qlearning::takeAction(int current_state, bool display){


    int action = getAction(current_state);
    int new_state = action;

    float current_sa_reward = 0;

    current_sa_reward = reward_matrix[current_state][action];

//    std::cout << "REWARD MATRIX" << std::endl;
//    for(int i = 0; i < numberOfStates; i++){
//        for(int j = 0; j < numberOfStates; j++){
//            std::cout << reward_matrix[i][j] << " ";
//        }
//       std::cout << std::endl;
//    }

    if(rooms[current_state].isVisited == true){
        current_sa_reward = 0;
    }

//    std::cout << "Room: " << current_state << " is " << rooms[current_state].isVisited << std::endl;
//    std::cout << current_sa_reward << std::endl;
    maxRewardRecieved += current_sa_reward;


    auto max = max_element(q_table[action].begin(), q_table[action].end());
    float future_sa_reward = *max;
    float Q_value_current_state = current_sa_reward + gamma * future_sa_reward;
    //std::cout << Q_value_current_state << std::endl;
    q_table[current_state][action] = Q_value_current_state;

    if(display){
        for (int state = 0; state < q_table[action].size(); state++) {
            for (int action = 0; action < q_table[action].size(); action++) {
                std::cout << q_table[state][action] << " ";
            }
            cout << endl;
        }
        cout << "Old state: " << current_state << " | " << "New state: " << new_state << endl;
    }



    rooms[current_state].isVisited = true;

    return new_state;

}



// Run one episode
//void Qlearning::runEpisode(bool display){
//    int current_state = initial_state_random;
//    while(true){
//         current_state = takeAction(current_state, display);
//         if(current_state == goal_state){
//             cout << "Goal Reached!" << endl;
//             break;
//         }
//    }
//}

// Run one episode
void Qlearning::runEpisode(bool display){
    int current_state = initial_state_random;
    int stepCount = 0;

    // Markov property - keeping track of which rooms have been visited
    for(int i = 0; i < numberOfStates; i++){
        rooms[i].roomNumber = i;
        rooms[i].isVisited = false;
    }

    while(true){
         current_state = takeAction(current_state, display);
         std::cout << "Cur: " << current_state << std::endl;
         stepCount++;
         if(stepCount == maxStepsPerEpisode){
             cout << "Episode ended!" << endl;
             cout << "Reward: " << maxRewardRecieved << endl;
             maxRewardRecieved = 0;
             break;
         }
    }
}



// Train the agent - Run several episodes
void Qlearning::train(bool display){

    initQTable();
    initRewardMatrix();

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
//void Qlearning::deployAgent(int goal){

//    goal_state = goal;
//    setRandomInitState();
//    int state = initial_state;
//    int action;
//    cout << "Start state: " << initial_state  << endl;
//    int steps = 0;

//    while(true){
//        steps++;
//        action = std::distance(q_table[state].begin(), max_element(q_table[state].begin(), q_table[state].end()));
//        cout << "Actions taken: " << action<< endl;
//        state = action;
//        if(state == goal_state){
//            cout << "Goal Reached!" << endl;
//            cout << "Number of Steps: " << steps << endl;
//            stepsToGoal = steps;
//            break;
//        }
//    }
//}



void Qlearning::deployAgent(int maxSteps){

    initial_state = 6;
    //setRandomInitState();
    int state = initial_state;
    int action;
    cout << "Start state: " << initial_state  << endl;
    int steps = 0;

    // Markov property - keeping track of which rooms have been visited
    for(int i = 0; i < numberOfStates; i++){
        rooms[i].roomNumber = i;
        rooms[i].isVisited = false;
    }

    while(true){
        steps++;
        action = std::distance(q_table[state].begin(), max_element(q_table[state].begin(), q_table[state].end()));
        if(rooms[state].isVisited == false){
            maxRewardRecieved += reward_matrix[state][action];
        }

        rooms[state].isVisited = true;
        cout << "Actions taken: " << action << endl;
        state = action;
        if(steps == maxSteps){
            cout << "Episode ended!" << endl;
            cout << "Max reward recieved: " << maxRewardRecieved << endl;
            maxRewardRecieved = 0;
            break;
        }
    }
}
