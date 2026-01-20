#pragma once
#include <string>

// 负责代码编译的对象
namespace sui
{
    extern std::string globalWorkPath;
    extern std::string logOutFile;
    class Compiler
    {
    public:
        Compiler(const std::string& workPath, const std::string& workCatalogue = globalWorkPath);
        ~Compiler();

        //进行本地编译
        bool compile(const std::string &sourceFileName, const std::string& outputFileName);
    private:
        // 本类唯一工作区域
        std::string _workCatalogue;
        std::string _logFilePath;
    };
}