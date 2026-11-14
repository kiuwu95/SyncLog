#pragma once
#include<chrono>
#include<string>
#include<ctime>
#include<stdio.h>

class GetTime {
public:
    GetTime(const GetTime&) = delete;
    GetTime& operator=(const GetTime&) = delete;

    GetTime() = delete;
    ~GetTime() = delete;

    static std::string RetCurrentTime() {
        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        tm tm;
        localtime_s(&tm, &time);

        char buf[32] = { 0 };
        snprintf(buf, sizeof(buf), "[%04d-%02d-%02d %02d:%02d:%02d]",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        return buf;
    }
};