#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
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