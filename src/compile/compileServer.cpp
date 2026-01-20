#include <iostream>
#include <unistd.h>
#include "log/log.hpp"

int main()
{
    srand((unsigned int)time(NULL));
    //进行日志系统测试
    std::cout << "日志系统文件测试开始: " << std::endl;
    suiLogSetFile("log.txt");
    suiLogInit("conpileServerLog");
    std::cout << "开始循环" << std::endl;
    for (int i = 0; i < 10000; ++i)
    {
        suiLog(sui::logLevel::debug) << "这是的第 " << i << " 条日志";
        sleep(rand() % 3);
    }
    return 0;
}