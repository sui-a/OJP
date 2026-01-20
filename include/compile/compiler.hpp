#pragma once
#include <string>


//负责代码编译的对象
namespace sui{
    class Compiler {
    public:
        Compiler();
        ~Compiler();

    private:
        //本类唯一工作区域
        std::string workArea;

    };
}