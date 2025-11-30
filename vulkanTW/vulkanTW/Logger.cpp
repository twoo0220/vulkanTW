#include "Logger.h"

std::unique_ptr<Logger> Logger::mInstance = nullptr;

Logger::Logger()
{
    mLogFile.open("log.txt", std::ios::out | std::ios::trunc);
    if (!mLogFile.is_open())
    {
        std::cerr << "ERROR : Could not open log.txt for writing!" << std::endl;
    }
}

Logger::~Logger()
{
    if (mLogFile.is_open())
    {
        mLogFile.flush();
        mLogFile.close();
    }
}

Logger& Logger::getInstance()
{
    if (!mInstance)
    {
        // std::make_unique cannot directly access Logger¡¯s private constructor internally.
        // so impossible std::make_unique<Logger>();
        mInstance = std::unique_ptr<Logger>(new Logger());
    }
    return *mInstance;
}

void Logger::printLog(std::string message)
{
    Logger& log = getInstance();
    //std::cout << message << std::endl;

    if (log.mLogFile.is_open())
    {
        log.mLogFile << message << std::endl;
        log.mLogFile.flush();
        log.mMessagesProcessd++;
    }
    else
    {
        std::cerr << "WARNING: Logfile is not open, message lost: " << message << std::endl;
    }
}

template<typename T>
void Logger::formatToStream(std::ostringstream& oss, T&& arg)
{
    oss << std::forward<T>(arg);
}

template<typename T, typename ...Args>
void Logger::formatToStream(std::ostringstream& oss, T&& first, Args && ...args)
{
    oss << std::forward<T>(first);
    if (sizeof...(args) > 0)
    {
        oss << " ";
        formatToStream(oss, std::forward<Args>(args)...);
    }
}

