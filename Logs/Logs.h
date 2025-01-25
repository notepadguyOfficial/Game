#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <mutex>

enum class LOG_LEVEL {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    HTTP,
    DATABASE
};

class Logs
{
public:
    static Logs& GetInstance();
    void SetLogFile(const std::string& filename);
    void SetLogLevel(LOG_LEVEL level);
    void Print(const char* fmt, LOG_LEVEL level, ...);
    void Initialized();

    template <typename T>
    void Input(T& input, const std::string& prompt)
    {
        std::cout << prompt;
        std::cin >> input;

        std::string temp = prompt + " " + std::to_string(input);

        Info(temp);
    }
    
    void Input(std::string& input, const std::string& prompt);

private:
    std::ofstream filestream_;
    std::mutex mutex_;
#if _DEBUG
    LOG_LEVEL LogLevel_ = LOG_LEVEL::DEBUG;
#else
    LOG_LEVEL LogLevel_ = LOG_LEVEL::INFO;
#endif
    Logs() = default;
    std::string GetCurrentTime();
    std::string FormatFileName();
    std::string FormatMessage(const std::string& message, LOG_LEVEL level);
};

#define LOG_DEBUG(fmt, ...) \
    Logs::Print(fmt, LOG_LEVEL::DEBUG, __VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    Logs::Print(fmt, LOG_LEVEL::INFO, __VA_ARGS__)

#define LOG_WARNING(fmt, ...) \
    Logs::Print(fmt, LOG_LEVEL::WARNING, __VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    Logs::Print(fmt, LOG_LEVEL::ERROR, __VA_ARGS__)

#define LOG_HTTP(fmt, ...) \
    Logs::Print(fmt, LOG_LEVEL::HTTP, __VA_ARGS__)

#define LOG_DATABASE(fmt, ...) \
    Logs::Print(fmt, LOG_LEVEL::DATABASE, __VA_ARGS__)

extern Logs& Log;