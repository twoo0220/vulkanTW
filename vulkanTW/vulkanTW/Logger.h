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

// Variadic template function to log an error message and terminate the program (C++11 compatible)
template<typename... Args>
void exitWithMessage(Args&&... args)
{
	std::ostringstream oss;

	// C++11 pack expansion trick using initializer list to unpack variadic arguments into the stream.
	// Each argument is appended with a trailing space.
	int dummy[] = { 0, ((oss << std::forward<Args>(args) << " "), 0)... };
	(void)dummy; // Prevent unused variable warning

	// Remove the trailing space if the message is not empty.
	std::string message = oss.str();
	if (!message.empty())
	{
		message.pop_back();
	}

	// Print the log and terminate the process.
	Logger::printLog(message);
	assert(false);
	exit(EXIT_FAILURE);
}

// std::format_string since C++20
//template<typename... Args>
//void printLog(std::format_string<Args...> fmt, Args&&... args)
//{
//	std::string message = std::format(fmt, std::forward<Args>(args)...);
//	Logger::printlog(message);
//}