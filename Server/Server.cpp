#include "Server.h"

#include <iostream>

int Server::clientId = 0;

Server::Server()
{
}

void Server::run()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStartup failed: " << iResult << std::endl;
		return;
	}
	else
	{
		std::cout << "WSAStartup successful" << std::endl;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "Failed to create socket: " << WSAGetLastError() << std::endl;
		return;
	}
	else
	{
		std::cout << "Created socket" << std::endl;
	}

	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(8084);

	int connResult = bind(serverSocket, (sockaddr*)&local, sizeof(local));

	if (connResult == SOCKET_ERROR)
	{
		std::cout << "Failed to bind socket: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		return;
	}
	else
	{
		std::cout << "Bind successful" << std::endl;
	}

	connResult = listen(serverSocket, 10);
	if (connResult == SOCKET_ERROR)
	{
		std::cout << "Failed to listen: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		return;
	}
	else
	{
		std::cout << "Listening" << std::endl;
	}

	sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET clientSocket;
	
	while (true)
	{
		clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "Failed to accept connection: " << WSAGetLastError() << std::endl;
			closesocket(serverSocket);
			return;
		}
		else
		{
			std::cout << "Connected" << std::endl;
		}


		m_clients[clientSocket].handler = std::thread([this, clientSocket]() { handleClient(clientSocket); });

	}

	for (auto& client : m_clients)
	{
		std::thread& clientHandler = client.second.handler;
		if (clientHandler.joinable())
		{
			clientHandler.join();
		}
	}

	closesocket(serverSocket);
	
	
	// = accept(acceptSocket, (sockaddr*)&clientAddr, &addrLen);
	//if (clientSocket == INVALID_SOCKET)
	//{
	//	std::cout << "Failed to accept connection: " << WSAGetLastError() << std::endl;
	//	closesocket(acceptSocket);
	//	return;
	//}
	//else
	//{
	//	std::cout << "Connected" << std::endl;
	//}

	//closesocket(acceptSocket);

	//char msgbuf[256];
	//char response[256];
	//int recvResult;
	//do
	//{
	//	recvResult = recv(clientSocket, msgbuf, 256, 0);
	//	if (recvResult == 0)
	//	{
	//		std::cout << "Closing connection" << std::endl;
	//		break;
	//	}
	//	else if (recvResult > 0)
	//	{
	//		msgbuf[recvResult] = '\0';
	//		std::cout << "Received message: " << msgbuf << std::endl;

	//		sprintf(response, "Received: %s", msgbuf);
	//		int sendResult = send(clientSocket, response, strlen(response), 0);
	//		if (sendResult == SOCKET_ERROR)
	//		{
	//			std::cout << "Failed to send response: " << WSAGetLastError() << std::endl;
	//			closesocket(clientSocket);
	//			return;
	//		}
	//		else
	//		{
	//			std::cout << "Sent response" << std::endl;
	//		}
	//	}
	//	else
	//	{
	//		std::cout << "Failed to receive message: " << WSAGetLastError() << std::endl;
	//		closesocket(clientSocket);
	//		return;
	//	}
	//} 	while (recvResult > 0);

	//int closeResult = shutdown(clientSocket, SD_SEND);
	//if (closeResult == SOCKET_ERROR)
	//{
	//	std::cout << "Failed to close connection: " << WSAGetLastError() << std::endl;
	//	closesocket(clientSocket);
	//	return;
	//}
	//else
	//{
	//	std::cout << "Closed connection" << std::endl;
	//}

	//closesocket(clientSocket);
	//WSACleanup();

	//return;

	//std::cout << "value: " << m_value << std::endl;
}

void Server::handleClient(SOCKET clientSocket)
{
	char messageBuf[BUF_SIZE];
	int status = recv(clientSocket, messageBuf, BUF_SIZE, 0);
	if (status == SOCKET_ERROR)
	{
		std::cout << "Failed to read name" << std::endl;
		endConnection(clientSocket);
	}

	std::string clientName(messageBuf);
	m_clients[clientSocket].name = clientName;

	std::string welcome = clientName + " has joined the chat.";
	broadcastMessage(welcome, clientSocket);
	std::cout << welcome << std::endl;

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
	int status;
	std::cout << message << std::endl;
	for (auto& client : m_clients)
	{
		if (client.first != senderSocket)
		{
			status = send(client.first, message.c_str(), message.length() + 1, 0);
			if (status <= 0)
			{
				std::cout << "something went wrong" << std::endl;
			}
		}
	}
}
