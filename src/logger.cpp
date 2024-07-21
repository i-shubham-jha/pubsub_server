#include "logger.h"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#define FILE_SIZE 5 * 1024 * 1024
#define NUM_FILES 3
#define FLUSH_INTERVAL 3

static std::shared_ptr<spdlog::logger> create_logger()
{
    std::shared_ptr<spdlog::logger> file_logger =
        spdlog::rotating_logger_mt<spdlog::async_factory>("logs", "logs/log.log", FILE_SIZE, NUM_FILES);

    spdlog::flush_every(std::chrono::seconds(FLUSH_INTERVAL));

#ifdef DEBUG_BUILD
    file_logger->set_level(spdlog::level::debug);
#endif

    return file_logger;
}

std::shared_ptr<spdlog::logger> file_logger = create_logger();
