#pragma once

#include <Logger.h>

#include <string>
#include <WinSock2.h>
#include <thread>

class Client
{
	static constexpr int BUF_SIZE = 512;

public:
	Client(const std::string& name, Logger::LogLevel logLevel = Logger::LogLevel::error);
	~Client();
	void connectToServer(const std::string& serverAddress, int port = 8084);

private:
	//void disconnectFromServer();

	void sendMessages();
	void readMessages();

private:
	std::string m_name;
	Logger m_log;

	bool m_connected;
	SOCKET m_connection;
	std::thread m_readThread;
	std::thread m_sendThread;
};