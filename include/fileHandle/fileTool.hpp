#pragma once
#include <string>

namespace sui
{
    /*
     *   文件工具
     */
    class fileTool
    {
    public:
        //检查路径是否存在
        static bool isFileExist(const std::string& path);
        //创建文件
        static bool createFile(const std::string& path);
    };
}