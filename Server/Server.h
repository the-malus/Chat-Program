#pragma once
#include <WinSock2.h>
#include <thread>
//#include <vector>
#include <unordered_map>

class Server
{
public:
	static constexpr int BUF_SIZE = 512;

	Server();
	void run();

private:
	void handleClient(SOCKET clientSocket);
	void endConnection(SOCKET clientSocket);
	void broadcastMessage(const std::string& message, SOCKET senderSocket);

	struct ClientData
	{
		std::string name;
		std::thread handler;
	};
	std::unordered_map<SOCKET, ClientData> m_clients;

	static int clientId;
};