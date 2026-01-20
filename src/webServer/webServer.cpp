#include <iostream>
#include <unistd.h>
#include <time.h>
#include "log/log.hpp"
#include "compile/compiler.hpp"

int main()
{
    //进行目录初始化工具测试
    suiLogInit("编译服务器测试程序");
    sui::Compiler com("8080");
    int a = 0;
    std::cin >> a;
    com.compile("main.cpp", "main.out");
    return 0;
}