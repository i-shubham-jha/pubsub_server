#pragma once

#include <cstdint>
#include <stdint.h>
#include <string>
#include <unordered_map>
/*
 * encapsulation for ds holding msgID per topic and
 * each msgID's count of subcriptions left to deliver
 * to, and related functions.
 * DS is a map from topic names to another map, which
 * goes from msgID to count of subcriptions left to
 * deliver to.
 * topics_directory should be the same supplied to Topics obj
 * msgID is unint64_t
 */
class Messages
{
    std::unordered_map<std::string, std::unordered_map<uint64_t, unsigned>> messages;
    std::string topics_directory;

    /*
     * used to generate uniq id for each mssg
     * currently uniq across all topics
     */
    uint64_t uniq_id;

    /* function to register a new topic in this ds */
    void new_topic(std::string const topic);

    /* function to check if the topic exists in this ds */
    bool exists_topic(std::string const topic);

  public:
    /* constructor with topics topics_directory */
    Messages(std::string const topics_directory);

    /* function to check if {topic, msgID} tuple exists or not */
    bool exists_mssg(std::string const topic, uint64_t const msgID);

    /* new message. returns msgID */
    uint64_t new_message(std::string const topic, std::string const mssg);

    /* get sub cnt for this msgID */
    unsigned get_sub_cnt(std::string topic, uint64_t msgID);

    /* increment sub cnt by x */
    void increment_sub_cnt(std::string topic, uint64_t const msgID, unsigned x);

    /* decrement sub cnt */
    void decrement_sub_cnt(std::string topic, uint64_t const msgID);

    /* delete this msgID */
    void delete_mssg(std::string const topic, uint64_t const msgID);
};
