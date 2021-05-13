#include "Client.h"
#include <Windows.h>
#include <iostream>

Client::Client(const std::string& name, Client::LogLevel logLevel /*= none*/)
	: m_name(name), m_log(logLevel), m_connected(false)
{
}

Client::~Client()
{
	//disconnectFromServer();
}

void Client::connectToServer(const std::string& serverAddress, int port)
{
	WSADATA wsaData;
	int status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0)
	{
		//std::cout << "WSAStartup failed: " << iResult << std::endl;
		log(error, "WSAStartup failed");
		return;
	}
	else
	{
		//std::cout << "WSAStartup successful" << std::endl;
		log(info, "WSAStartup successful");
	}

	m_connection = socket(AF_INET, SOCK_STREAM, 0);
	if (m_connection == INVALID_SOCKET)
	{
		//std::cout << "Failed to create socket: " << WSAGetLastError() << std::endl;
		log(error, "Failed to create socket");
		return;
	}
	else
	{
		//std::cout << "Created socket" << std::endl;
		log(info, "Created socket");
	}

	struct sockaddr_in serverSockAddr;
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_port = htons(port);
	serverSockAddr.sin_addr.s_addr = inet_addr(serverAddress.c_str());
	status = connect(m_connection, (sockaddr*)&serverSockAddr, sizeof(serverSockAddr));

	if (status == SOCKET_ERROR)
	{
		//std::cout << "Failed to connect to server: " << WSAGetLastError() << std::endl;
		log(error, "Failed to connect to server");
		closesocket(m_connection);
		return;
	}
	else
	{
		//std::cout << "Connection successful" << std::endl;
		log(info, "Connected to server");
	}

	m_sendThread = std::thread([this]() { sendMessages(); });
	m_readThread = std::thread([this]() { readMessages(); });

	m_connected = true;

	//m_readerThread = new std::thread([this]()
	//	{
	//		readMessages();
	//	});

	//char msg[256];
	//char* prefix = "sent from ";
	//strcpy(msg, prefix);
	//strcat(msg, m_name.c_str());
	//int sendResult = send(m_connection, msg, (int)strlen(msg), 0);
	//
	//if (sendResult == SOCKET_ERROR)
	//{
	//	std::cout << "Failed to send message" << std::endl;
	//	closesocket(m_connection);
	//	return;
	//}
	//else
	//{
	//	std::cout << "Sent message: " << msg << std::endl;
	//}

	//int recvResult = recv(m_connection, msg, 256, 0);
	//if (recvResult == SOCKET_ERROR)
	//{
	//	std::cout << "Failed to receive response: " << WSAGetLastError() << std::endl;
	//	closesocket(m_connection);
	//	return;
	//}
	//else
	//{
	//	msg[recvResult] = '\0';
	//	std::cout << "Received response: " << msg  << std::endl;
	//}

	//for (int i = 0; i < 10; ++i)
	//{
	//	sprintf(msg, "message %d", i);
	//	sendResult = send(m_connection, msg, strlen(msg), 0);

	//	if (sendResult == SOCKET_ERROR)
	//	{
	//		std::cout << "Failed to send message" << std::endl;
	//		closesocket(m_connection);
	//		return;
	//	}
	//	else
	//	{
	//		std::cout << "Sent message: " << msg << std::endl;
	//	}

	//	int recvResult = recv(m_connection, msg, 256, 0);
	//	if (recvResult == SOCKET_ERROR)
	//	{
	//		std::cout << "Failed to receive response: " << WSAGetLastError() << std::endl;
	//		closesocket(m_connection);
	//		return;
	//	}
	//	else
	//	{
	//		msg[recvResult] = '\0';
	//		std::cout << "Received response: " << msg << std::endl;
	//	}
	//}

	//closesocket(m_connection);
	//WSACleanup();

	if (m_sendThread.joinable())
	{
		m_sendThread.join();
	}
	if (m_readThread.joinable())
	{
		m_readThread.join();
	}

	return;

	//std::cout << "value: " << m_value << std::endl;
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
	char messageBuf[BUF_SIZE];
	std::string message;
	while (true)
	{
		
		std::getline(std::cin, message);
		status = send(m_connection, message.c_str(), message.size() + 1, 0);
		if (status == SOCKET_ERROR)
		{
			m_connected = false;
			m_readThread.detach();
			closesocket(m_connection);
			return;
		}
	}

	//if (status == SOCKET_ERROR)
	//{
	//	log(error, "Failed to send message");
	//}
	//else
	//{
	//	log(info, "message sent: " + message);
	//}

	//char msg[256];
	//int recvResult = recv(m_connection, msg, 256, 0);
	//if (recvResult == SOCKET_ERROR)
	//{
	//	std::cout << "Failed to receive response: " << WSAGetLastError() << std::endl;
	//	closesocket(m_connection);
	//	return;
	//}
	//else
	//{
	//	msg[recvResult] = '\0';
	//	std::cout << "Received response: " << msg << std::endl;
	//}
}

void Client::readMessages()
{
	char messageBuf[BUF_SIZE];
	int status;
	while (m_connected)
	{
		char recBuf[BUF_SIZE];
		status = recv(m_connection, messageBuf, BUF_SIZE, 0);
		if (status <= 0)
		{
			continue;
		}

		//log(info, std::string(messageBuf));
		std::cout << messageBuf << std::endl;

		//if (status == SOCKET_ERROR)
		//{
		//	log(error, "Failed to receive message");
		//	break;
		//}
		//else if (status == 0)
		//{
		//	log(warning, "Disconnecting");
		//	break;
		//}
		//else
		//{
		//	log(info, "message received: " + std::string(recBuf));
		//}
	}
}

void Client::log(LogLevel logLevel, const std::string& logMessage) const
{
	if (logLevel < m_log) return;

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (logLevel)
	{
	case warning:
		SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	case error:
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
	case info:
	default:
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	std::cout << logMessage << std::endl;
}
