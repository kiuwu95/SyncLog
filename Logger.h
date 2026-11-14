#pragma once
#include<string>
#include<memory>
#include<iostream>
#include<fstream>
#include<utility>
#include<vector>
#include<deque>
#include<thread>
#include<mutex>
#include<atomic>
#include<condition_variable>

#include"LEVEL.h"
#include"Formatter.h"

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    Logger(const std::string& log_file, const LEVEL& min_level = LEVEL::INFO);
    ~Logger();

    void PushLog(const LEVEL& level, const std::string log);

    template<class... Args>
    void Log(const LEVEL& level, const std::string& str, Args&& ... args) {
        PushLog(level, Formatter::RetString(level, str, std::forward<Args>(args)...));
    }

    void SetLogFile(const std::string& log_file);
    void SetMinLevel(const LEVEL& min_level) { min_level_ = min_level; }
private:
    void Start();
private:
    std::vector<std::string> log_queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::unique_ptr<std::ofstream> writer_;
    std::thread writer_thread_;


    std::atomic<bool> end_ = false;
    std::atomic<size_t> producer_index_ = 0;
    std::string log_file_;
    std::atomic<LEVEL> min_level_;
};