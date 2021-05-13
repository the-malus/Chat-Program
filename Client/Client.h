#pragma once

#include <string>
#include <WinSock2.h>
#include <thread>

class Client
{
	static constexpr int BUF_SIZE = 512;

public:
	enum LogLevel {
		none,
		info,
		warning,
		error
	};
	Client(const std::string& name, LogLevel logLevel = error);
	~Client();
	void connectToServer(const std::string& serverAddress, int port = 8084);

private:
	//void disconnectFromServer();

	void sendMessages();
	void readMessages();

	void log(LogLevel logLevel, const std::string& logMessage) const;

private:
	std::string m_name;
	LogLevel m_log;

	bool m_connected;
	SOCKET m_connection;
	std::thread m_readThread;
	std::thread m_sendThread;
};