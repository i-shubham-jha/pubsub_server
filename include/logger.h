#pragma once

#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"

extern std::shared_ptr<spdlog::logger> file_logger;

#define WARN_LOG file_logger->warn
#define INFO_LOG file_logger->info
#define ERROR_LOG file_logger->error
#define CRITIAL_LOG file_logger->critical
#define DEBUG_LOG file_logger->debug
