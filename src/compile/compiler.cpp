#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include "log/log.hpp"
#include "compile/compiler.hpp"
#include "pathHandle/pathTool.hpp"
using namespace sui;

std::string sui::globalWorkPath = "./temp/WorkSpace/";
std::string sui::logOutFile = "error.log";
Compiler::Compiler(const std::string &workPath, const std::string &workCatalogue)
    : _workCatalogue(workCatalogue)
{
    // 对工作区进行创建
    while (_workCatalogue.back() == '/')
    {
        _workCatalogue.pop_back();
    }
    
    _workCatalogue += "/" + workPath;
    _logFilePath = _workCatalogue + "/"+ logOutFile;
    
    // 创建并初始化工作目录
    sui::pathTool::createPaths(_workCatalogue);
    // 不确定原本是否存在，直接清除
    sui::pathTool::pathInit(_workCatalogue);
    suiLog(logLevel::info) << "初始化编译工作区: " << _workCatalogue;
}

Compiler::~Compiler()
{

}

bool Compiler::compile(const std::string &sourceFileName, const std::string &outputFileName)
{
    // 根据工作区编译
    int forkReturn = fork();
    if (forkReturn < 0)
    {
        // 创建失败
        // 进行失败流程处理
        return false;
    }
    else if (forkReturn == 0)
    {
        // 成功创建子进程
        // 进行进程替换工作
        // 替换标准输出流
        int retOut = open(_logFilePath.c_str(), O_WRONLY | O_CREAT, 0644);
        if(retOut == -1)
        {
            suiLog(logLevel::error) << "打开日志文件失败，无法进行输出重定";
        }
        dup2(retOut, STDOUT_FILENO);
        dup2(retOut, STDERR_FILENO);
        close(retOut);
        
        int rel = execlp("g++", "g++", "-o",
               (_workCatalogue + "/" + outputFileName).c_str(),
               (_workCatalogue + "/" + sourceFileName).c_str(),
               nullptr);
        
        if(rel == -1)
        {
            suiLog(logLevel::error) << "进程替换执行失败";
        }
        exit(0);
    }

    // 等待进程结束
    if(wait(&forkReturn) == -1)
    {
        //等待失败，直接退出
        return false;
    }
    suiLog(logLevel::info) << "编译结束";
    return false;
}
