#include "messages.h"
#include "logger.h"
#include <filesystem>
#include <fstream>
#include <string>
namespace fs = std::filesystem;
/*
 * init topics_directory
 * keep it in sync with what is supplied to Topics obj
 */
Messages::Messages(std::string const topics_directory)
{
    this->topics_directory = topics_directory;
    this->uniq_id = 0;
}

/*
 * check if this topic is there in this ds or not.
 * not in sync with Topics class.
 */
bool Messages::exists_topic(std::string const topic)
{
    return (messages.find(topic) != messages.end());
}

/*
 * check if this {topic, msgID} tuple exists or not
 */
bool Messages::exists_mssg(std::string const topic, uint64_t const msgID)
{
    return (exists_topic(topic) && messages[topic].find(msgID) != messages[topic].end());
}

/*
 * function to register a new topic in this ds.
 * Has no sync with the Topics obj, hdd direcotory also
 * not created here.
 */
void Messages::new_topic(std::string const topic)
{
    /* don't create topic if already there */
    if (!exists_topic(topic))
    {
        /* insert this topic with emtpy msgID container */
        messages.insert({topic, {}});
        INFO_LOG("Added new topic {}", topic);
    }
    else
    {
        WARN_LOG("Not creating already existing topic {}", topic);
    }
}

/*
 * function to receive a new mssg published to a topic,
 * persist it on hdd, assign a uniq msgID to this mssg,
 * and return this msgID.
 * Assumes that the directory on hdd has been created
 * by calling the Topics obj.
 */
uint64_t Messages::new_message(std::string const topic, std::string const mssg)
{
    /* is this a new topic? */
    if (!exists_topic(topic))
    {
        new_topic(topic);
    }

    /* uniq msgID. Increment after assigning to current mssg */
    uint64_t msgID = uniq_id;
    uniq_id++;
    messages[topic].insert({msgID, 0});

    /* persist on hdd */
    std::string file_path = topics_directory + "/" + topic + "/" + std::to_string(msgID) + ".txt";

    std::ofstream fout(file_path, std::fstream::out);

    /* If cannot persist mssg on hdd, purpose defeated
     * might happen if holding directory for this topic
     * has not been created by using Topics class.
     */
    if (!fout.is_open())
    {
        CRITIAL_LOG("Cannot open file {}. Exiting...", file_path);
        exit(-1);
    }
    else
    {
        fout << mssg;
    }
    fout.close();

    return msgID;
}

/*
 * increment sub cnt by x
 * receives msgID only
 */
void Messages::increment_sub_cnt(std::string topic, uint64_t const msgID, unsigned x)
{
    /* check if topic there or not */
    if (exists_topic(topic))
    {
        /* check if msgID there or not */
        if (exists_mssg(topic, msgID))
        {
            messages[topic][msgID] += x;
        }
        else
        {
            WARN_LOG("No {} msgID for {} topic", msgID, topic);
        }
    }
    else
    {
        WARN_LOG("incrementing sub cnt in non-existent topic {}", topic);
    }
}

/*
 * decrement sub cnt
 * receives msgID only
 */
void Messages::decrement_sub_cnt(std::string topic, uint64_t msgID)
{
    /* check if topic there or not */
    if (exists_topic(topic))
    {
        /* check if msgID there or not */
        if (exists_mssg(topic, msgID))
        {
            messages[topic][msgID]--;
        }
        else
        {
            WARN_LOG("No {} msgID for {} topic", msgID, topic);
        }
    }
    else
    {
        WARN_LOG("decrementing sub cnt in non-existent topic {}", topic);
    }
}

/*
 * delete this msgID from ds as well as hdd
 * only deletes when sub cnt for this mssg is zero
 */
void Messages::delete_mssg(std::string const topic, uint64_t msgID)
{
    /* check if topic there or not */
    if (exists_topic(topic))
    {
        /* check if msgID there or not */
        if (exists_mssg(topic, msgID))
        {
            /* only delete if sub cnt == 0 */
            if (messages[topic][msgID] == 0)
            {
                /* remove from ds */
                messages[topic].erase(msgID);

                /* remove from hdd */
                fs::path path = topics_directory + "/" + topic + "/" + std::to_string(msgID) + ".txt";

                if (fs::exists(path))
                {
                    fs::remove(path);
                }
                else
                {
                    WARN_LOG("Mssg in ds but not on hdd for ({}, {})", topic, msgID);
                }
            }
            else
            {
                WARN_LOG("Sub cnt not zero for ({}, {})", topic, msgID);
            }
        }
        else
        {
            WARN_LOG("Deleting non-existent ({}, {})", topic, msgID);
        }
    }
    else
    {
        WARN_LOG("Deleting msgID in non-existent topic {}", topic);
    }
}
