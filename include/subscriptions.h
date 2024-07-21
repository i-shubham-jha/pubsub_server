#ifndef SUBSCRIPTIONS_H
#define SUBSCRIPTIONS_H

#include <cstdint>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

/* for brevity */
#define umm std::unordered_map
#define uss std::unordered_set

/*
 * encapsulation for the ds holding, per topic, the list of subIDs and
 * list of msgIDs left to deliver to each subID.
 * Map from topic to another map. Second map from subID to set of msgIDs.
 */

class Subscriptions
{
    umm<std::string, umm<uint64_t, uss<uint64_t>>> subscriptions;

    /*
     * used to gen uniq subID for each subscription.
     * id unique across all subscriptions.
     */
    uint64_t uniq_id;

    /* does this topic exist in ds */
    bool exists_topic(std::string topic);

    /* create new topic locally in ds */
    void new_topic(std::string topic);

    /* does this subID exist under this topic */
    bool exists_sub_id(std::string topic, uint64_t subID);

  public:
    Subscriptions();

    /* creates a new sub for this topic. returns new subID */
    uint64_t new_sub(std::string topic);

    /* returns number of active subs for this topic */
    uint64_t subs_cnt(std::string topic);

    /* add this msgID to all subs of this topic */
    void add_msg_id(std::string topic, uint64_t msgID);

    /* get all msgIDs of messages to be delivered to this subID */
    uss<uint64_t> get_mssg_ids(std::string topic, uint64_t subID);

    /* clear all msgIDs from this subID */
    void clear_all_mssgs(std::string topic, uint64_t subID);

    /* check if any sub there for topic */
    bool exists_subs(std::string topic);
};

#endif
