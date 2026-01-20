#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "log/log.hpp"
#include "fileHandle/fileTool.hpp"
#include "pathHandle/pathTool.hpp"
using namespace sui;

const std::string sui::logFileOutPathDefault = "./temp/Log"; // 默认日志文件输出路径
const std::string sui::logFileNameDefault = "log.txt";
sui::logController sui::myLogController;

std::string sui::LeToS(logLevel level)
{
    switch (level)
    {
    case logLevel::debug:
        return "DEBUG";
    case logLevel::info:
        return "INFO";
    case logLevel::warn:
        return "WARN";
    case logLevel::error:
        return "ERROR";
    case logLevel::fatal:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
}

std::string sui::getTime()
{
    // 1. 获取当前系统时间戳
    std::time_t now = std::time(nullptr);

    // 2. 转换为本地时间结构体 (tm)
    // 注意：std::localtime 返回的是内部静态指针，非线程安全
    std::tm *localTime = std::localtime(&now);

    // 3. 使用 stringstream 进行格式化
    std::stringstream ss;
    if (localTime)
    {
        // %Y=年, %m=月, %d=日, %H=时, %M=分, %S=秒
        ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    }
    else
    {
        return "Unknown Time";
    }

    return ss.str();
}

void logStdout::output(const std::string &logString)
{
    std::cout << logString << std::endl;
}

logFile::logFile(const std::string &fileName, const std::string &path)
    : _path(path), _fileName(fileName)
{
    if (_path.empty() || fileName.empty())
    {
        // 为空传入错误，直接返回
        return;
    }

    // 判断路径结尾是否存在/，存在则去除
    while (_path.back() == '/')
    {
        _path.pop_back();
    }
    // 判断文件名开头是否存在/ 或者.
    {
        int pos = 0;
        while (_fileName[pos] == '/' || _fileName[pos] == '.')
            ++pos;

        if (pos != 0)
            _fileName = _fileName.substr(pos);
    }
    // 根据目录手动创建目录
    sui::pathTool::createPaths(_path);
    // 创建文件
    sui::fileTool::createFile(_path + "/" + _fileName);

    _fd = open((_path + "/" + _fileName).c_str(), O_WRONLY | O_APPEND);
}

void logFile::output(const std::string &logString)
{
    // 文件输出逻辑待实现
    // 上锁+重定位偏移
    flock(_fd, LOCK_EX);
    lseek(_fd, 0, SEEK_END); // 跳转到结尾
    write(_fd, logString.c_str(), logString.size());
    flock(_fd, LOCK_UN);
}

logFile::~logFile()
{
    if (_fd != -1)
    {
        close(_fd);
        _fd = -1;
    }
}

logController::logController()
    : _outputter(nullptr)
{
    // 进行初始化
    // 默认标准输出
    setStdoutLog();
}

logController::~logController()
{
    if (_outputter)
    {
        // 指针不为空进行重置
        _outputter.reset();
    }
}

void logController::setStdoutLog()
{
    if (_outputter)
    {
        // 指针不为空进行重置
        _outputter.reset();
    }

    // 构造输出类
    std::unique_ptr<logStdout> _newOutPutter(new logStdout());
    _outputter = std::move(_newOutPutter);
}

void logController::stdFileLog(const std::string &fileName)
{
    if (_outputter)
    {
        // 指针不为空进行重置
        _outputter.reset();
    }

    // 构造输出类
    std::unique_ptr<logFile> _newOutPutter(new logFile(fileName));
    _outputter = std::move(_newOutPutter);
}

void logController::Init(const std::string &progmaName)
{
    //
    _progmaName = progmaName;
}

void logController::getName(std::string &progmaName) const
{
    progmaName = _progmaName;
}

std::string logController::getName() const
{
    return _progmaName;
}

logController::logStreamer::logStreamer(logController *parent, logLevel level, const char *file, int line)
    : _parnet(parent)
{
    // 进行日志头构造
    auto filename = parent->getName();
    //[程序名][文件名][行号][日志等级][时间]:
    _logString = "[" + _parnet->getName() + "]";
    _logString += "[" + filename + "]";
    _logString += "[" + std::to_string(line) + "]";
    _logString += "[" + LeToS(level) + "]";
    // 获取时间
    _logString += "[" + getTime() + "]: ";
}

logController::logStreamer::~logStreamer()
{
    std::cout << "进入析构" << std::endl;
    _logString += "\n";
    _parnet->logOutPut(_logString);
}

logController::logStreamer &logController::logStreamer::operator<<(const std::string &logString)
{
    _logString += logString;
    return *this;
}
logController::logStreamer& logController::logStreamer::operator<<(const int &logInt)
{
    _logString += std::to_string(logInt);
    return *this;
}

void logController::logOutPut(const std::string &logString)
{
    if (_outputter)
    {
        // 不为空可以正常输出
        _outputter->output(logString);
    }
}

logController::logStreamer logController::getLogStreamer(logLevel level, const char *file, int line)
{
    return logStreamer(this, level, file, line);
}