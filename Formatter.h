#pragma once
#include<string>
#include<sstream>
#include<utility>

#include"LEVEL.h"
#include"GetTime.h" 
#include"Logger.h"
class Logger;

class Formatter {
public:
    Formatter(const Formatter&) = delete;
    Formatter& operator=(const Formatter&) = delete;

    Formatter() = delete;
    ~Formatter() = delete;

    template<class ... Args>
    static std::string RetString(const LEVEL& level, const std::string& str, Args&& ... args) {
        return LevelToString(level) + GetTime::RetCurrentTime() +
            StringFormatter(str, std::forward<Args>(args)...);
    }
private:
    static std::string LevelToString(const LEVEL& level) {
        switch (level) {
        case LEVEL::DEBUG:
            return "[DEBUG]";
        case LEVEL::ERROR:
            return "[ERROR]";
        case LEVEL::INFO:
            return "[INFO]";
        case LEVEL::WARN:
            return "[WARN]";
        default:
            return "[UNKOWN]";
        }
    }

    template<class... Args>
    static std::string StringFormatter(const std::string& str, Args&& ... args) {
        std::vector<std::string> arr{ ArgToString<Args...>(std::forward<Args>(args)...) };

        std::ostringstream oos;
        size_t args_index = 0, left = 0;
        size_t pos = str.find("{}");
        while (pos < str.size()) {
            oos << str.substr(left, pos - left);

            if (args_index < arr.size()) {
                oos << arr[args_index++];
            }
            else {
                oos << "{}";
            }

            pos += 2;
            left = pos;
            pos = str.find("{}", pos);
        }
        oos << str.substr(left);

        return oos.str();
    }

    template<class T>
    static std::string ArgToString(T&& arg) {
        return std::to_string(std::forward<T>(arg));
    }
};