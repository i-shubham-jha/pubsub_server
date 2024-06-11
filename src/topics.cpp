#include "logger.h"
#include <filesystem>
#include <topics.h>

namespace fs = std::filesystem;

// constructor
// inits the topics_directory var with the directory where
// topics and mssgs should be stored
// also creates the holding directory for topics and mssgs
Topics::Topics()
{
    this->topics_directory = "./messages";

    fs::path path = topics_directory;

    if (!fs::exists(path))
    {
        fs::create_directory(path);
    }
    INFO_LOG("Topics object init done");
}

bool Topics::check_topic(std::string const topic)
{
    return topics_list.find(topic) != topics_list.end();
}

void Topics::create_topic(std::string const topic)
{
    if (!check_topic(topic))
    {
        topics_list.insert(topic);
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

void Topics::delete_topic(std::string const topic)
{
    // TODO: delete topic from list as well its directory
}
