#pragma once

#include <Logger.h>

#include <WinSock2.h>
#include <thread>
#include <unordered_map>

class Server
{
public:
	static constexpr int BUF_SIZE = 512;

	Server(Logger::LogLevel logLevel = Logger::LogLevel::error);
	void run();

private:
	void acceptClients(SOCKET serverSocket);
	void handleClient(SOCKET clientSocket);
	void endConnection(SOCKET clientSocket);
	void broadcastMessage(const std::string& message, SOCKET senderSocket);
	void sendMessage(SOCKET clientSocket, const std::string& message, bool log = false);

private:
	struct ClientData
	{
		std::string name;
		std::thread handler;
	};
	std::unordered_map<SOCKET, ClientData> m_clients;

	Logger m_log;
};