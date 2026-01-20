#include <iostream>
#include <unistd.h>
#include "log/log.hpp"
#include "pathHandle/pathTool.hpp"

int main()
{
    //进行目录初始化工具测试
    sui::pathTool::pathInit("./temp/WorkSpace/8080");
    std::cout << "成功初始化工作区" << std::endl;
    return 0;
}