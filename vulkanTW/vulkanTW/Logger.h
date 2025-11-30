// Logger class from HongLab

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <cassert>
#include <string>
//#include <format> // since C++20

class Logger
{
public:
	~Logger();
	static Logger& getInstance();
	static void printLog(std::string message);

private:
	Logger();
	Logger(const Logger&) = delete;
	Logger& operator= (const Logger&) = delete;

	template<typename T>
	void formatToStream(std::ostringstream& oss, T&& arg);

	template<typename T, typename... Args>
	void formatToStream(std::ostringstream& oss, T&& first, Args&&... args);
	
private:
	static std::unique_ptr<Logger> mInstance;
	std::ofstream mLogFile;
	size_t mMessagesProcessd = 0;
};


// std::format_string since C++20
//template<typename... Args>
//void printLog(std::format_string<Args...> fmt, Args&&... args)
//{
//	std::string message = std::format(fmt, std::forward<Args>(args)...);
//	Logger::printlog(message);
//}