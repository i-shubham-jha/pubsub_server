#include "subscriptions.h"
#include "logger.h"

Subscriptions::Subscriptions() : uniq_id(0)
{
}

/*
 * does this topic exist in ds
 */
bool Subscriptions::exists_topic(std::string topic)
{
    return (subscriptions.find(topic) != subscriptions.end());
}

/*
 * create new topic locally in ds.
 * Init with empty list of subIDs
 */
void Subscriptions::new_topic(std::string topic)
{
    subscriptions[topic];
}

/* does this subID exist under this topic */
bool Subscriptions::exists_sub_id(std::string topic, uint64_t subID)
{
    return (exists_topic(topic) && (subscriptions[topic].find(subID) != subscriptions[topic].end()));
}

/*
 * creates a new sub for this topic. returns new subID
 * new sub has empty msgID list in the beginnning.
 */
uint64_t Subscriptions::new_sub(std::string topic)
{
    /*
     * if topic dne, warn but create it.
     */
    if (!exists_topic(topic))
    {
        WARN_LOG("creating subID for non-existent topic {}", topic);
        new_topic(topic);
    }

    /* new sub ID starting at zero. */
    uint64_t subID = uniq_id;
    uniq_id++;

    /* new sub with empty msgID list */
    subscriptions[topic][subID];
    INFO_LOG("created new subID {} for topic {}", subID, topic);
    return subID;
}

/*
 * returns number of active subs for this topic
 */
uint64_t Subscriptions::subs_cnt(std::string topic)
{
    if (exists_topic(topic))
    {
        return subscriptions[topic].size();
    }
    else
    {
        ERROR_LOG("count asked for non-existent topic {}", topic);
        return 0u;
    }
}

/*
 * add this msgID to all subs of this topic.
 */
void Subscriptions::add_msg_id(std::string topic, uint64_t msgID)
{
    if (!exists_topic(topic))
    {
        WARN_LOG("Adding msgID to non-existent topic {}", topic);
    }
    else
    {
        for (auto &p : subscriptions[topic])
        {
            p.second.push_back(msgID);
        }
    }
}

/*
 * get all msgIDs of messages to be delivered to this subID
 * Returns zero if topic/subID absent.
 */
std::vector<uint64_t> Subscriptions::get_mssg_ids(std::string topic, uint64_t subID)
{
    if (!exists_topic(topic))
    {
        ERROR_LOG("Getting messages for non-existent topic {}", topic);
        return {};
    }
    else if (!exists_sub_id(topic, subID))
    {
        ERROR_LOG("Getting messages for non-existent subID: ({}, {})", topic, subID);
        return {};
    }
    else
    {
        return subscriptions[topic][subID];
    }
}

/*
 * clear all msgIDs from this subID
 */
void Subscriptions::clear_all_mssgs(std::string topic, uint64_t subID)
{
    if (!exists_topic(topic))
    {
        ERROR_LOG("Clearing messages for non-existent topic {}", topic);
    }
    else if (!exists_sub_id(topic, subID))
    {
        ERROR_LOG("Clearing non-existent {} subID's messages for topic {}", subID, topic);
    }
    else
    {
        subscriptions[topic][subID].clear();
    }
}

/*
 * check if any sub there for topic or not
 */
bool Subscriptions::exists_subs(std::string topic)
{
    return ((subscriptions.find(topic) != subscriptions.end()) && (subscriptions[topic].size() > 0));
}
