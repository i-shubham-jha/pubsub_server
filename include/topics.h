#pragma once

// Topics is a separate class with helper functions
#include <string>
#include <unordered_map>

// encapsulation for the topics list ds and corresponding functions.
// This class maintains a map from the topic name to the number of
// messages currently there in the system for that topic.
// See cpp file for description of functions
class Topics
{
    std::unordered_map<std::string, unsigned> topics;
    std::string topics_directory;

  public:
    Topics();

    // checks if a topic there or not
    inline bool exists(std::string const topic);

    // create a new topic
    void create_topic(std::string const topic);

    // delete topic
    void delete_topic(std::string const topic, bool delete_directory);

    // increment message count
    void increment_mssg_cnt(std::string const topic);

    // decrement message count
    void decrement_mssg_cnt(std::string const topic);

    // get message count
    unsigned get_mssg_cnt(std::string const topic);
};
