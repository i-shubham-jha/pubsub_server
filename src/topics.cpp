#include "topics.h"
#include "logger.h"
#include <filesystem>

namespace fs = std::filesystem;

/*
 * constructor
 * inits the topics_directory var with the directory where
 * topics and mssgs should be stored.
 * Also creates the holding directory for topics and mssgs on hdd.
 */
Topics::Topics(std::string const topics_directory) : topics_directory(topics_directory)
{
    fs::path path = this->topics_directory;

    if (!fs::exists(path))
    {
        fs::create_directory(path);
    }
    INFO_LOG("Topics object init done");
}

/*
 *function to find whether the given topic exists in the ds or not
 */
bool Topics::exists(std::string const topic)
{
    return topics.find(topic) != topics.end();
}

/*
 * function to create a new topic
 * also creates the directory for this new topic
 */
void Topics::create_topic(std::string const topic)
{
    if (!exists(topic))
    {
        /* inserting into map */
        topics[topic] = 0;

        /* creating directory for this topic */
        fs::path path = (topics_directory + "/" + topic);
        if (!fs::exists(path))
        {
            fs::create_directory(path);
        }
        else
        {
            WARN_LOG("directory already there for topic {}", topic);
        }

        INFO_LOG("new topic created: {}", topic);
    }
    else
    {
        WARN_LOG("{} topic already there", topic);
    }
}

/*
 * function to delete a topic.
 * removes the associated directory as well if second arg is true.
 */
void Topics::delete_topic(std::string const topic, bool const delete_directory)
{
    if (exists(topic))
    {
        topics.erase(topic);

        /* deleting the entire topics directory */
        if (delete_directory)
        {
            fs::path path = (topics_directory + "/" + topic);

            if (fs::exists(path))
            {
                fs::remove_all(path);
            }
        }
    }
    else
    {
        WARN_LOG("deleting non-existent topic {}", topic);
    }
}

/*
 * increments the counter of messages for this topic by 1
 */
void Topics::increment_mssg_cnt(std::string const topic)
{
    if (exists(topic))
    {
        topics[topic]++;
    }
    else
    {
        WARN_LOG("incrementing non-existent topic {}", topic);
    }
}

/*
 * decrements the counter of messages for this topic by 1
 */
void Topics::decrement_mssg_cnt(std::string const topic)
{
    if (exists(topic))
    {
        if (topics[topic] > 0)
        {
            topics[topic]--;
        }
        else
        {
            WARN_LOG("can't decrement zero cnt for topic {}", topic);
        }
    }
    else
    {
        WARN_LOG("decrementing non-existent topic {}", topic);
    }
}

/*
 * returns count of messages for this topic.
 * returns 0 even if topics DNE. Ideally check if topic
 * exists before callling this function.
 */
unsigned Topics::get_mssg_cnt(std::string const topic)
{
    if (exists(topic))
    {
        return topics[topic];
    }
    else
    {
        WARN_LOG("get_mssg_cnt called for non-existent topic {}", topic);
        return 0;
    }
}
