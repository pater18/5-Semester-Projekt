#include <iostream>
#include <qlearning.h>

using namespace std;

int main()
{
    Qlearning ql(20000);

    ql.train();

    ql.displayTrainedQTable();

    ql.deployAgent2();

    return 0;
}
