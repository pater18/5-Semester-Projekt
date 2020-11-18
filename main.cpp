#include <iostream>
#include <qlearning.h>

using namespace std;

int main()
{
    Qlearning ql(1000);

    ql.initQTable();
    ql.initRewardMatrix();

    //cout << ql.get_action(5) << endl;

    //cout << ql.take_action(ql.get_action(10), true);

    //ql.runEpisode(false);

    ql.train(false);

    ql.displayTrainedQTable();

    //ql.setGoalState(1);

    ql.deployAgent(10);

    return 0;
}
