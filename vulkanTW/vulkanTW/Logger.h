// Logger class from HongLab

#pragma once

#include <vulkan/vulkan.h>
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

	void checkVulkanResult(VkResult result);

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

template <typename... Args>
void printLog(Args&&... args)
{
	std::ostringstream oss;

	// C++11 pack expansion trick using initializer list to unpack variadic arguments into the stream.
	// Each argument is appended with a trailing space.
	int dummy[] = { 0, ((oss << std::forward<Args>(args) << " "), 0)... };
	(void)dummy; // Prevent unused variable warning

	std::string message = oss.str();

	// Remove the trailing space if the message is not empty.
	if (!message.empty()) {
		message.pop_back();
	}

	// Print the log.
	Logger::printLog(message);
}

// Vulkan logger utility function
std::string getResultString(VkResult result);
