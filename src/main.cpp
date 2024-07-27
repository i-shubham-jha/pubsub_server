#include "crow/app.h"
#include "crow/common.h"
#include "crow/http_response.h"
#include "crow/json.h"
#include "crow/logging.h"
#include "logger.h"
#include "messages.h"
#include "subscriptions.h"
#include "topics.h"
#include <cstdint>
#include <json.hpp>
#include <mutex>
#include <string>
#include <unordered_set>

/* GLOBAL CONSTANTS */
/* directory for storing the topics's folders */
std::string const topics_directory = "data_store";

/* port to run server on */
unsigned const PORT = 3000;

int main()
{
    /* helper objects */
    Topics topics_ds(topics_directory);
    Messages mssgs_ds(topics_directory);
    Subscriptions subs_ds;

    /* mutex objects */
    static std::mutex topics_mtx;
    static std::mutex mssgs_mtx;
    static std::mutex subs_mtx;

    /* webserver app */
    crow::SimpleApp app;

    /* log level from the crow web server */
    app.loglevel(crow::LogLevel::WARNING);

    /* ROUTE for new message */
    CROW_ROUTE(app, "/new_message/<string>/<string>")
        .methods(crow::HTTPMethod::POST)(
            [&topics_ds, &mssgs_ds, &subs_ds](std::string topic_arg, std::string message_arg) {
                /* if no sub for this topic, ignore this message */
                bool sub_present = true;

                subs_mtx.lock();
                sub_present = subs_ds.exists_subs(topic_arg);
                subs_mtx.unlock();

                if (!sub_present)
                {
                    /* no sub, no need to persist this mssg */
                    return "Successful reception of message";
                }

                /* sub exists for this topic */

                /* incrementing message count for this topic */
                topics_mtx.lock();

                if (!topics_ds.exists(topic_arg))
                {
                    topics_ds.create_topic(topic_arg);
                }

                topics_ds.increment_mssg_cnt(topic_arg);

                topics_mtx.unlock();

                /* creating new message */
                mssgs_mtx.lock();
                uint64_t mssgID = mssgs_ds.new_message(topic_arg, message_arg);
                mssgs_mtx.unlock();

                /* adding this message to all subs for this topic */
                subs_mtx.lock();
                subs_ds.add_msg_id(topic_arg, mssgID);
                uint64_t subs_cnt = subs_ds.subs_cnt(topic_arg);
                subs_mtx.unlock();

                /* message needs to be delivered to these messages also */
                mssgs_mtx.lock();
                mssgs_ds.increment_sub_cnt(topic_arg, mssgID, subs_cnt);
                mssgs_mtx.unlock();

                return "Successful reception of message";
            });

    /* route for new sub request */
    CROW_ROUTE(app, "/new_sub/<string>").methods(crow::HTTPMethod::POST)([&subs_ds](std::string topic_arg) {
        crow::json::wvalue res;

        /* create new sub and subID */
        subs_mtx.lock();
        uint64_t sub_id = subs_ds.new_sub(topic_arg);
        subs_mtx.unlock();

        res["subID"] = sub_id;

        return res;
    });

    /* route for new pull request */
    CROW_ROUTE(app, "/get_messages/<string>/<uint>")
    ([&mssgs_ds, &subs_ds](crow::response &res, std::string topic_arg, uint64_t sub_id_arg) {
        /* getting and clearing the message ids for this sub */
        subs_mtx.lock();
        std::unordered_set<uint64_t> mssg_ids = subs_ds.get_mssg_ids(topic_arg, sub_id_arg);
        subs_ds.clear_all_mssgs(topic_arg, sub_id_arg);
        subs_mtx.unlock();

        /* do messages even exist? */
        if (mssg_ids.size() == 0)
        {
            res.code = 204;
            res.end();
            return;
        }

        /* decrement all mssgIDs of this subID */
        mssgs_mtx.lock();
        for (uint64_t msg_id : mssg_ids)
        {
            mssgs_ds.decrement_sub_cnt(topic_arg, msg_id);
        }
        mssgs_mtx.unlock();

        /* creating and sending the reponse as a js array */
        std::ifstream fin;
        std::string mssg;
        std::string path = topics_directory + "/" + topic_arg;

        nlohmann::json j;
        j["messages"] = nlohmann::json::array();

        for (uint64_t msg_id : mssg_ids)
        {
            fin.open(path + "/" + std::to_string(msg_id) + ".txt");

            if (fin.is_open())
            {
                fin >> mssg;
                j["messages"].push_back(mssg);
            }
            else
            {
                ERROR_LOG("Unable to open file for response: {}", path + "/" + std::to_string(msg_id) + ".txt");
            }

            fin.close();
        }

        res.set_header("Content-Type", "application/json");
        res.write(j.dump());
        res.end();
    });

    /* no of threads as hardware_concurrency() */
    app.port(PORT).multithreaded().run();
}
