#pragma once

#include <cstdint>
#include <stdint.h>
#include <string>
#include <unordered_map>
/*
 * encapsulation for ds holding mssgID per topic and
 * each mssgID's count of subcriptions left to deliver
 * to, and related functions.
 * DS is a map from topic names to another map, which
 * goes from mssgID to count of subcriptions left to
 * deliver to.
 * topics_directory should be the same supplied to Topics obj
 * mssgID is unint64_t
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

  public:
    /* constructor with topics topics_directory */
    Messages(std::string const topics_directory);

    /* function to check if the topic exists in this ds */
    bool exists_topic(std::string const topic);

    /* function to check if {topic, mssgID} tuple exists or not */
    bool exists_mssg(std::string const topic, uint64_t const mssgID);

    /* function to register a new topic in this ds */
    void new_topic(std::string const topic);

    /* new message. returns mssgID */
    uint64_t new_message(std::string const topic, std::string const mssg);

    /* increment sub cnt by x */
    void increment_sub_cnt(std::string topic, uint64_t const mssgID, unsigned x);

    /* decrement sub cnt */
    void decrement_sub_cnt(std::string topic, uint64_t const mssgID);

    /* delete this mssgID */
    void delete_mssg(std::string const topic, uint64_t const mssgID);
};
