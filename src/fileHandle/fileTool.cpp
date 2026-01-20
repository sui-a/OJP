#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "fileHandle/fileTool.hpp"

bool sui::fileTool::isFileExist(const std::string &path)
{
    int fd = open(path.c_str(), O_RDONLY, 0664);  //读方式打开不会破坏原有文件内容，不需要加O_APPEND
    if (fd != -1)
    {
        // 执行成功，无论是原本就有还是新创建的
        close(fd); // 别忘了关闭文件描述符，宝宝
        return true;
    }

    // 2. 如果失败了，我们要判断原因
    return false;
}

bool sui::fileTool::createFile(const std::string &path)
{
    //无法在不存在的路径中创建文件
    int fd = open(path.c_str(), O_RDONLY | O_CREAT, 0664);
    if (fd != -1)
    {
        // 执行成功，无论是原本就有还是新创建的
        close(fd); // 别忘了关闭文件描述符，宝宝
        return true;
    }

    // 2. 如果失败了，我们要判断原因
    return false;
}