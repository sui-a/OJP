#pragma once 
#include <string>


namespace sui
{
    /*
        路径处理工具
    */
    class pathTool
    {
    public:
        //判断路径是否存在
        static bool isPathExist(const std::string& path);
        
        //进行路径创建
        static bool createPaths(const std::string& path);

        //工作目录初始化
        static bool pathInit(const std::string &path);
    };
}