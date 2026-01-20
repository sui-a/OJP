#pragma once
#include <string>
#include <memory>

namespace sui
{
    /*
     *   日志工具
     */
    extern const std::string logFileOutPathDefault; // 默认日志文件输出路径
    extern const std::string logFileNameDefault;
    enum class logLevel
    {
        debug = 0, // debug日志
        info,      // 正常日志
        warn,      // 警告日志
        error,     // 错误日志
        fatal      // 严重错误日志
    };
    //切换成string类型
    std::string LeToS(logLevel level);
    //获取时间
    std::string getTime();

    // 输出基类
    class logBase
    {
    public:
        logBase() = default;
        virtual ~logBase() = default;

        // 输出类
        virtual void output(const std::string &logString) = 0;
    };

    // 标准输出流指定的日志输出类
    class logStdout : public logBase
    {
    public:
        logStdout() = default;
        ~logStdout() override = default;

        void output(const std::string &logString) override;
    };

    // 文件输出控制
    class logFile : public logBase
    {
    public:
        logFile(const std::string &fileName = logFileNameDefault, const std::string &path = logFileOutPathDefault);
        ~logFile();

        void output(const std::string &logString) override;

    private:
        int _fd = -1;
        std::string _path;
        std::string _fileName;
    };

    // 日志控制类
    class logController
    {
    public:
        //日志流处理类
        class logStreamer
        {
        public:
            logStreamer(logController* parent, logLevel level, const char* file, int line); //传入日志控制类指针和日志等级, 调用的文件， 行号
            logStreamer& operator<<(const std::string& logString);
            logStreamer& operator<<(const int& logInt);
            ~logStreamer();
        private:
            std::string _logString;
            logController* _parnet;
        };

    public:
        /*
         * 控制为流输出模式
         */
        logController();
        ~logController();

        void Init(const std::string &progmaName = "");
        void getName(std::string &progmaName) const;
        std::string getName() const;

        //修改为标准输出日志
        void setStdoutLog();
        //修改为文件输出日志
        void stdFileLog(const std::string& fileName);

        //进行输出
        void logOutPut(const std::string& logString);
        //获取日志流处理类
        logStreamer getLogStreamer(logLevel level, const char* file, int line);

        
    private:
        //使用日志系统的服务
        std::string _progmaName;
        //输出类
        std::unique_ptr<logBase> _outputter;
    };
    extern logController myLogController;

#define suiLog(LEVEL) sui::myLogController.getLogStreamer(LEVEL, __FILE__, __LINE__)
#define suiLogInit(PRAGMA_NAME) sui::myLogController.Init(PRAGMA_NAME)
#define suiLogSetStdout() sui::myLogController.setStdoutLog()
#define suiLogSetFile(FILE_NAME) sui::myLogController.stdFileLog(FILE_NAME)
}