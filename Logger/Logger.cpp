#include "Logger.h"

#include <Windows.h>
#include <iostream>

Logger::Logger(LogLevel logLevel)
	: m_logLevel(logLevel)
{
}

void Logger::log(LogLevel logLevel, const std::string& logMessage)
{
	if (logLevel < m_logLevel) return;

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (logLevel)
	{
	case LogLevel::warning:
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "[Warning] ";
		break;
	}
	case LogLevel::error:
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "[Error] ";
		break;
	}
	case LogLevel::info:
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "[Info] ";
		break;
	}
	case LogLevel::message:
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "[Message] ";
		break;
	}
	default:
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}
	}

	std::cout << logMessage << std::endl;

	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
