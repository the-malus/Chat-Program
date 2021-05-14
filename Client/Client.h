#pragma once

#include <Logger.h>

#include <string>
#include <WinSock2.h>
#include <thread>
#include <functional>

class Client
{
	static constexpr int BUF_SIZE = 512;

public:
	Client(Logger::LogLevel logLevel = Logger::LogLevel::none);
	~Client();

	void setName(const std::string& name);
	void connectToServer(const std::string& serverAddress, int port = 8084);

	void sendMessage(const std::string& message);
	void listenToMessageReceived(std::function<void(const std::string&)> callbackFunction);

	bool isConnected() const;

private:
	void disconnectFromServer();
	void readMessages();

private:
	std::string m_name;
	Logger m_log;

	bool m_connected;
	SOCKET m_connection;
	std::thread m_readThread;

	std::function<void(const std::string&)> m_messageListener;
};