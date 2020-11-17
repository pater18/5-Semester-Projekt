#include <iostream>
#include <qlearning.h>

using namespace std;

int main()
{
    Qlearning ql;

    ql.init_qTable();
    ql.init_reward_matrix();

    return 0;
}
