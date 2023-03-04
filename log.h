#pragma once
// This header offers macros for easy usage of spdlog. (See https://github.com/gabime/spdlog)
// spdlog installation: vcpkg.exe install spdlog:x64-windows-static-md
// 
// Usage:
// ######
// Before including this header set SPDLOG_ACTIVE_LEVEL and then include spdlog/spdlog.h (See https://spdlog.docsforge.com/v1.x/0.faq/#how-to-remove-all-debug-statements-at-compile-time  and  https://spdlog.docsforge.com/v1.x/0.faq/#source-information-do-not-appear-when-using-custom-format
// User code must initialize the loggers and register the default logger.
// Look for "logging.h" and "logging.cpp" in our projects for examples.

#ifndef SPDLOG_H
#ifndef SPDLOG_ACTIVE_LEVEL
#pragma message("!!!!!!!!!!WARNING!!!!!!!!!!: SPDLOG_ACTIVE_LEVEL is not defined. Defining it to be SPDLOG_LEVEL_TRACE by default.")
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif // !SPDLOG_ACTIVE_LEVEL
#pragma message("!!!!!!!!!!WARNING!!!!!!!!!!: SPDLOG_H is not defined. Meaning spdlog/spdlog.h was not included. It will be included here...")
#include "spdlog/spdlog.h"
#endif // SPDLOG_H

// Write to the default logger registered by calling spdlog::register_logger function in the initializing code
#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)


// Log using a specific log. the "to" argument should be something like:
// auto l = spdlog::get(LOGGER_NAME__FILE_ONLY);
// or
// std::shared_ptr<spdlog::logger> cl = spdlog::get(LOGGER_NAME__CONSOLE_ONLY);
#define LOG2_TRACE(to, ...) if(to) SPDLOG_LOGGER_TRACE(to, __VA_ARGS__)
#define LOG2_DEBUG(to, ...) if(to) SPDLOG_LOGGER_DEBUG(to, __VA_ARGS__)
#define LOG2_INFO(to, ...) if(to) SPDLOG_LOGGER_INFO(to, __VA_ARGS__)
#define LOG2_WARN(to, ...) if(to) SPDLOG_LOGGER_WARN(to, __VA_ARGS__)
#define LOG2_ERROR(to, ...) if(to) SPDLOG_LOGGER_ERROR(to, __VA_ARGS__)
#define LOG2_CRITICAL(to, ...) if(to) SPDLOG_LOGGER_CRITICAL(to, __VA_ARGS__)

#define ERROR_MSG_HW_EXCEPTION "Hardware exception such as Access Violation or Division by 0."

#define LOG_FUNC_START LOG_TRACE("Started...");
#define LOG_FUNC_DONE LOG_TRACE("Done.");