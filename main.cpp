#include"Logger.h"

static Logger logger("E:\\桌面\\编程\\项目\\SyncLog\\info.log");

void func1() {
    for (int i = 0;i < 10000;i++) {
        logger.Log(LEVEL::INFO, "func1 send message {}", i);
    }
}
void func2() {
    for (int i = 0;i < 10000;i++) {
        logger.Log(LEVEL::ERROR, "func2 error {}", i);
    }
}
void func3() {
    for (int i = 0;i < 10000;i++) {
        logger.Log(LEVEL::WARN, "func3 warn {}", i);
    }
}
int main() {
    std::thread t1(func1);
    std::thread t2(func2);
    std::thread t3(func3);

    t1.join();
    t2.join();
    t3.join();
}