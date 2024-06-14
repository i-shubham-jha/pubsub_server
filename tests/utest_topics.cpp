#include "logger.h"
#include "topics.h"
#include <iostream>
using namespace std;
int main()
{
    INFO_LOG("Starting unit test program for topics");

    Topics x("messages_utest_topics");

    cout << x.get_mssg_cnt("t1") << endl;

    x.delete_topic("t1", true);

    x.increment_mssg_cnt("t1");
    x.decrement_mssg_cnt("t1");

    x.create_topic("t1");

    cout << x.get_mssg_cnt("t1") << endl;

    x.create_topic("t2");
    x.create_topic("t3");
    x.create_topic("t4");
    x.create_topic("t5");
    x.create_topic("t6");
    x.create_topic("t7");
    x.create_topic("t8");

    for (int i = 0; i < 10; i++)
    {
        x.increment_mssg_cnt("t1");
        x.increment_mssg_cnt("t2");
    }

    cout << x.get_mssg_cnt("t1") << endl;

    x.delete_topic("t1", true);
}
