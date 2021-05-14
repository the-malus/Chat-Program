#include "Server.h"

Server::Server(Logger::LogLevel logLevel)
	: m_log(logLevel)
{
}

void Server::run()
{
	m_log.log(Logger::LogLevel::info, "Initializing");

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		m_log.log(Logger::LogLevel::error, "WSAStartup failed");
		return;
	}
	else
	{
		m_log.log(Logger::LogLevel::info, "WSAStartup successful");
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		m_log.log(Logger::LogLevel::error, "Failed to create socket");
		return;
	}
	else
	{
		m_log.log(Logger::LogLevel::info, "Created socket");
	}

	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(8084);

	int connResult = bind(serverSocket, (sockaddr*)&local, sizeof(local));

	if (connResult == SOCKET_ERROR)
	{
		m_log.log(Logger::LogLevel::error, "Failed to bind socket");
		closesocket(serverSocket);
		return;
	}
	else
	{
		m_log.log(Logger::LogLevel::info, "Bind successful");
	}

	connResult = listen(serverSocket, 10);
	if (connResult == SOCKET_ERROR)
	{
		m_log.log(Logger::LogLevel::error, "Failed to listen");
		closesocket(serverSocket);
		return;
	}
	else
	{
		m_log.log(Logger::LogLevel::info, "Listening for clients");
	}

	acceptClients(serverSocket);

	for (auto& client : m_clients)
	{
		std::thread& clientHandler = client.second.handler;
		if (clientHandler.joinable())
		{
			clientHandler.join();
		}
	}

	closesocket(serverSocket);

	return;
}

void Server::acceptClients(SOCKET serverSocket)
{
	sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET clientSocket;

	while (true)
	{
		clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			m_log.log(Logger::LogLevel::warning, "Failed to connect to client");
			continue;
		}
		else
		{
			m_log.log(Logger::LogLevel::info, "Connected to client");
		}


		m_clients[clientSocket].handler = std::thread([this, clientSocket]() { handleClient(clientSocket); });

	}
}

void Server::handleClient(SOCKET clientSocket)
{
	char messageBuf[BUF_SIZE];
	int status = recv(clientSocket, messageBuf, BUF_SIZE, 0);
	if (status == SOCKET_ERROR)
	{
		m_log.log(Logger::LogLevel::warning, "Failed to receive client name");
		endConnection(clientSocket);
		return;
	}

	std::string clientName(messageBuf);
	m_clients[clientSocket].name = clientName;

	std::string introduction = clientName + " has joined the chat.";
	broadcastMessage(introduction, clientSocket);

	std::string welcome = "Welcome to the chat, " + clientName;
	sendMessage(clientSocket, welcome, true);

	while (true)
	{
		status = recv(clientSocket, messageBuf, BUF_SIZE, 0);
		if (status <= 0)
		{
			std::string goodbye = clientName + " has left the chat.";
			broadcastMessage(goodbye, clientSocket);
			endConnection(clientSocket);
			return;
		}
		else if (status == 1)
		{
			continue;
		}

		std::string message = clientName + ": " + std::string(messageBuf);
		broadcastMessage(message, clientSocket);
	}

}

void Server::endConnection(SOCKET clientSocket)
{
	m_clients[clientSocket].handler.detach();
	m_clients.erase(clientSocket);
	closesocket(clientSocket);
}

void Server::broadcastMessage(const std::string& message, SOCKET senderSocket)
{
	m_log.log(Logger::LogLevel::message, message);

	for (auto& client : m_clients)
	{
		if (client.first != senderSocket)
		{
			sendMessage(client.first, message, false);
		}
	}
}

void Server::sendMessage(SOCKET clientSocket, const std::string& message, bool log)
{
	if (log)
	{
		m_log.log(Logger::LogLevel::message, message);
	}

	int status = send(clientSocket, message.c_str(), int(message.length()) + 1, 0);
	if (status <= 0)
	{
		m_log.log(Logger::LogLevel::warning, "Failed to send message to client");
	}
}
