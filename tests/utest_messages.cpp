#include "messages.h"
#include "test_utils.h"
#include "topics.h"
#include <cstdint>
#include <iostream>

int main()
{
    std::string topics_directory = "messages_utest_messages";

    Topics topics_ds(topics_directory);
    Messages mssg_ds(topics_directory);

    new_test("message in non-existent topic");
    std::cout << mssg_ds.exists_mssg("t1", 2) << "\n";

    new_test("ops on mssg in non-existent topic");
    mssg_ds.increment_sub_cnt("t1", 1, 2);
    mssg_ds.decrement_sub_cnt("t1", 2);
    mssg_ds.delete_mssg("t1", 3);

    // new_test("new mssg with non-existent topic directory");
    // mssg_ds.new_message("t2", "some_message");

    new_test("new mssg with existent topic directory");
    topics_ds.create_topic("t3");
    mssg_ds.new_message("t3", "this\tshould\nbe\nsaved\non\nhdd.\n");

    new_test("stress test for adding new messages");
    topics_ds.create_topic("stress_test");

    for (int i = 1; i <= 1000; i++)
    {
        uint64_t uuid = mssg_ds.new_message("stress_test", gen_random_str(1000));
        if (uuid % 100 == 0)
        {
            mssg_ds.increment_sub_cnt("stress_test", uuid, 10);
        }
    }

    new_test("Deleting mssg with zero/non-zero sub counts");
    for (int i = 1; i <= 1000; i++)
    {
        mssg_ds.delete_mssg("stress_test", i);
    }
}
