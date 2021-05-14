#include "Client.h"
//#include <Windows.h>
#include <WS2tcpip.h>
#include <iostream>

Client::Client(const std::string& name, Logger::LogLevel logLevel /*= none*/)
	: m_name(name), m_log(logLevel), m_connected(false), m_connection()
{
}

Client::~Client()
{
	//disconnectFromServer();
}

void Client::connectToServer(const std::string& serverAddress, int port)
{
	m_log.log(Logger::LogLevel::info, "Initializing");

	WSADATA wsaData;
	int status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0)
	{
		m_log.log(Logger::LogLevel::error, "WSAStartup failed");
		return;
	}
	else
	{
		m_log.log(Logger::LogLevel::info, "WSAStartup successful");
	}

	m_connection = socket(AF_INET, SOCK_STREAM, 0);
	if (m_connection == INVALID_SOCKET)
	{
		m_log.log(Logger::LogLevel::error, "Failed to create socket");
		return;
	}
	else
	{
		m_log.log(Logger::LogLevel::info, "Created socket");
	}

	struct sockaddr_in serverSockAddr;
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_port = htons(port);
	inet_pton(AF_INET, serverAddress.c_str(), &serverSockAddr.sin_addr);
	status = connect(m_connection, (sockaddr*)&serverSockAddr, sizeof(serverSockAddr));

	if (status == SOCKET_ERROR)
	{
		m_log.log(Logger::LogLevel::error, "Failed to connect to server");
		closesocket(m_connection);
		return;
	}
	else
	{
		m_log.log(Logger::LogLevel::info, "Connected to server");
	}

	status = send(m_connection, m_name.c_str(), int(m_name.length()) + 1, 0);

	m_readThread = std::thread([this]() { readMessages(); });
	m_connected = true;

	sendMessages();

	if (m_readThread.joinable())
	{
		m_readThread.join();
	}

	return;
}

//void Client::disconnectFromServer()
//{
//	if (m_connected)
//	{
//		closesocket(m_connection);
//		WSACleanup();
//
//		m_connected = false;
//		if (m_readerThread)
//		{
//			m_readerThread->join();
//			delete m_readerThread;
//			m_readerThread = 0;
//		}
//
//	}
//}

void Client::sendMessages()
{
	int status;
	std::string message;
	while (true)
	{
		std::getline(std::cin, message);
		status = send(m_connection, message.c_str(), int(message.size()) + 1, 0);
		if (status == SOCKET_ERROR)
		{
			m_connected = false;
			m_readThread.detach();
			closesocket(m_connection);
			return;
		}
	}
}

void Client::readMessages()
{
	char messageBuf[BUF_SIZE];
	int status;
	while (m_connected)
	{
		status = recv(m_connection, messageBuf, BUF_SIZE, 0);
		if (status <= 0)
		{
			continue;
		}

		m_log.log(Logger::LogLevel::message, messageBuf);
	}
}
