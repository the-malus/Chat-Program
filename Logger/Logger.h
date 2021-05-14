#pragma once

#include <string>

class Logger
{
public:
	enum class LogLevel
	{
		all,
		info,
		warning,
		error,
		message,
		none
	};
	Logger(LogLevel logLevel);

	void log(LogLevel logLevel, const std::string& logMessage);

private:
	LogLevel m_logLevel;
};