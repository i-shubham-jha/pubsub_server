#include "logger.h"
#include "spdlog/common.h"
#define FILE_SIZE 5 * 1024 * 1024
#define NUM_FILES 3

static std::shared_ptr<spdlog::logger> create_logger()
{
    std::shared_ptr<spdlog::logger> file_logger =
        spdlog::rotating_logger_mt<spdlog::async_factory>("logs", "logs/log.log", FILE_SIZE, NUM_FILES);

#ifdef DEBUG_BUILD
    file_logger->set_level(spdlog::level::debug);
#endif

    return file_logger;
}

std::shared_ptr<spdlog::logger> file_logger = create_logger();
