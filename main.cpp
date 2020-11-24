#include <iostream>
#include <qlearning.h>

using namespace std;

int main()
{
    Qlearning ql(10000);

    //ql.initQTable();
    //ql.initRewardMatrix();

    //cout << ql.get_action(5) << endl;

    //cout << ql.take_action(ql.get_action(10), true);

    //ql.runEpisode(true);

    ql.train(false);

    ql.displayTrainedQTable();

    ql.deployAgent(5);

    return 0;
}
