#include"Logger.h"

Logger::Logger(const std::string& log_file, const LEVEL& min_level) :
    writer_(std::make_unique<std::ofstream>()),
    log_file_(log_file),
    min_level_(min_level) {
    writer_->open(log_file, std::ios::out | std::ios::app);
    writer_thread_ = std::thread([this] {Start();});
}
Logger::~Logger() {
    end_ = true;

    cv_.notify_one();
    if (writer_thread_.joinable()) {
        writer_thread_.join();
    }

    writer_->close();
}

void Logger::PushLog(const LEVEL& level, const std::string log) {
    if (level < min_level_) { return; }
    {
        std::unique_lock<std::mutex> lock(mtx_);
        log_queue_.emplace_back(std::move(log));
    }
    cv_.notify_one();
}

void Logger::SetLogFile(const std::string& log_file) {
    {
        std::unique_lock<std::mutex> lock(mtx_);
        log_file_ = log_file;
    }
    if (!writer_->is_open()) {
        writer_->open(log_file_, std::ios::out | std::ios::app);
    }
}

void Logger::Start() {
    while (true) {
        std::vector<std::string> logs{};
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, [this] {return !log_queue_.empty() || end_;});

            if (log_queue_.empty() && end_) { break; }

            if (!writer_->is_open()) {
                std::cout << "Failed to open the file..." << std::endl;
                writer_->close();
                break;
            }
            logs.swap(log_queue_);
        }


        for (auto& it : logs) {
            *writer_ << it << '\n';
        }

        writer_->flush();
    }
}