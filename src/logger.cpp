#include "logger.h"
#define FILE_SIZE 5 * 1024 * 1024
#define NUM_FILES 3

std::shared_ptr<spdlog::logger> file_logger =
    spdlog::rotating_logger_mt<spdlog::async_factory>("logs", "logs/log.log", FILE_SIZE, NUM_FILES);
