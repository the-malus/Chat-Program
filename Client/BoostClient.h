#pragma once

#include <string>
#include <asio.hpp>

class BoostClient
{
public:
	enum LogLevel {
		logMessage,
		logError,
		logWarning,
		logInfo
	};
	BoostClient(const std::string& name, LogLevel logLevel = logMessage);
	~BoostClient();

	void connectToServer(const std::string& serverAddress, const std::string& port);

	void disconnectFromServer();

	void sendMessage(const std::string& message);

private:
	void readMessages();

	void log(LogLevel logLevel, const std::string& message) const;

private:
	std::string m_name;
	LogLevel m_log;

	asio::io_context m_ioContext;
	asio::ip::tcp::socket m_socket;
	std::thread* m_ioContextThread;
};