#pragma once

#include <Logger.h>

#include <string>
#include <WinSock2.h>
#include <thread>
#include <functional>

class Client
{
	static constexpr int BUF_SIZE = 512;

	//typedef void (*callback_function)(const std::string&);

public:
	Client(const std::string& name, Logger::LogLevel logLevel = Logger::LogLevel::error);
	~Client();
	void connectToServer(const std::string& serverAddress, int port = 8084);

	void sendMessage(const std::string& message);
	void listenToMessageReceived(std::function<void(const std::string&)> callbackFunction);

	bool isConnected() const;

private:
	void disconnectFromServer();

	void sendMessages();
	void readMessages();

private:
	std::string m_name;
	Logger m_log;

	bool m_connected;
	SOCKET m_connection;
	std::thread m_readThread;
	std::thread m_sendThread;

	std::function<void(const std::string&)> m_messageListener;
};