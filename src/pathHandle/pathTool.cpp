#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <dirent.h>
#include "pathHandle/pathTool.hpp"

bool sui::pathTool::isPathExist(const std::string &path)
{
    struct stat buffer; // 文件状态结构体
    if (stat(path.c_str(), &buffer) == 0)
    {
        // 成功读取到文件属性
        return true;
    }

    return false;
}

bool sui::pathTool::createPaths(const std::string &path)
{
    size_t pos = 0;
    std::string current_path;

    while ((pos = path.find_first_of('/', pos + 1)) != std::string::npos)
    {
        current_path = path.substr(0, pos);
        // 如果路径为空（比如路径以 / 开头），跳过
        if (current_path.empty())
            continue;

        // 尝试创建这一级目录
        if (mkdir(current_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
        {
            // 如果报错原因不是“目录已存在”，则返回失败
            if (errno != EEXIST)
                return false;
        }
    }

    // 最后创建目标目录本身
    if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
    {
        if (errno != EEXIST)
            return false;
    }
    return true;
}

bool sui::pathTool::pathInit(const std::string &path)
{
    //
    // 1. 打开目录
    std::cout << "进入" << path << "路径进行删除 " << std::endl;
    DIR *dir = opendir(path.c_str());
    if (dir == nullptr)
    {
        // 目录不存在或是无法打开 直接返回
        return false;
    }

    struct dirent *entry;
    // 2. 循环读取目录项
    while ((entry = readdir(dir)) != nullptr)
    {
        // 1. 严格跳过 . 和 ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        std::string fullPath = path + "/" + entry->d_name;
        struct stat statbuf;
        
        if (lstat(fullPath.c_str(), &statbuf) == -1)
        {
            perror("lstat error");
            continue;
        }

        // 2. 判断类型
        if (S_ISDIR(statbuf.st_mode))
        {
            // 如果是目录：先递归清空它里面的东西
            pathInit(fullPath); 
            
            // 【关键修改】：递归回来后，子目录已空，必须调用 rmdir 把它删掉
            // 这样才能达到“全部删除”的效果
            if (rmdir(fullPath.c_str()) != 0)
            {
                perror("rmdir child dir error");
            }
        }
        else
        {
            // 如果是文件或软链接：直接删除
            if (unlink(fullPath.c_str()) != 0)
            {
                perror("unlink error");
                continue;
            }
        }
    }

    // 5. 关闭目录句柄
    closedir(dir);
    return true;
}