#include "qlearning.h"


using namespace std;

Qlearning::Qlearning()
{

}



// Initialize the q-table and the environment
void Qlearning::init_qTable(){

    vector<vector<float> > q_table_init(numberOfStates,std::vector<float>(numberOfStates, 0.0));


    q_table = q_table_init;

//    for(int i = 0; i < numberOfStates; i++){
//        for(int j = 0; j < numberOfStates; j++){
//            std::cout << q_table[i][j] << " ";
//        }
//       std::cout << std::endl;
//    }

}


void Qlearning::init_reward_matrix(){

    vector<int> room1 = {-1, 20, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    vector<int> room2 = {-1, -1, 5, -1, -1, 0, -1, -1, -1, -1, -1};
    vector<int> room3 = {-1, 20, -1, 5, 15, 0, -1, -1, -1, -1, -1};
    vector<int> room4 = {-1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1};
    vector<int> room5 = {-1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1};
    vector<int> room6 = {-1, 20, 5, -1, -1, -1, 15, 5, 5, 5, -1};
    vector<int> room7 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
    vector<int> room8 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
    vector<int> room9 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1};
    vector<int> room10 = {-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 20};
    vector<int> room11 = {-1, -1, -1, -1, -1, -1, -1, -1, -1, 5, -1};

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

    for(int i = 0; i < numberOfStates; i++){
        for(int j = 0; j < numberOfStates; j++){
            std::cout << reward_matrix[i][j] << " ";
        }
       std::cout << std::endl;
    }

}



// Get random init state
void Qlearning::get_random_init_state(){



}



// Get and take an action
int Qlearning::take_action(){



}


int Qlearning::get_action(){



}



// Run one episode
void Qlearning::episode(){



}


// Train the agent - Run several episodes
void Qlearning::train(){




}
