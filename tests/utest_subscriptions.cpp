/*
 * test file for subscriptions DS module
 */

#include "subscriptions.h"
#include "test_utils.h"
#include <cstdint>
#include <vector>

int main()
{
    Subscriptions subs;

    new_test("Non-existent topic");
    subs.clear_all_mssgs("fish", 686);
    subs.get_mssg_ids("fish", 4545);
    subs.add_msg_id("fish", 44234);
    std::cout << "subs_cnt = " << subs.subs_cnt("t1") << "\n";

    new_test("creating new_sub with msgID");
    uint64_t new_sub_id = subs.new_sub("boat");
    subs.add_msg_id("boat", new_sub_id);

    new_test("Stress test: adding subs and mssgs");
    std::vector<uint64_t> new_subs;
    for (int i = 1; i <= 1000; i++)
    {
        new_subs.push_back(subs.new_sub("stress_test"));
        subs.add_msg_id("stress_test", i);
    }

    for (int i = 999; i >= 995; i--)
    {
        uint64_t sub_id = new_subs[i];
        std::cout << sub_id << ": ";
        uss<uint64_t> ss = subs.get_mssg_ids("stress_test", sub_id);
        for (uint64_t msg_id : ss)
        {
            std::cout << msg_id << ", ";
        }
        std::cout << std::endl;
    }

    for (int i : new_subs)
    {
        std::cout << subs.subs_cnt(topic);
    }
}
