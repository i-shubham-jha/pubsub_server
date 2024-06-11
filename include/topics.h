// Topics is a separate class with helper functions
#include <string>
#include <unordered_set>

class Topics
{
    std::unordered_set<std::string> topics_list;
    std::string topics_directory;

  public:
    Topics();

    // checks if a topic there or not
    bool check_topic(std::string const topic);

    // create a new topic
    void create_topic(std::string const topic);

    // delete topic
    void delete_topic(std::string const topic);
};
