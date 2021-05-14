#include "Client.h"

#include <WS2tcpip.h>

Client::Client(Logger::LogLevel logLevel)
	: m_name("no name"), m_log(logLevel), m_connected(false), m_connection()
{
}

Client::~Client()
{
	disconnectFromServer();
}

void Client::setName(const std::string& name)
{
	m_name = name;
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

	return;
}

void Client::sendMessage(const std::string& message)
{
	if (m_connected)
	{
		int status = send(m_connection, message.c_str(), int(message.size()) + 1, 0);
		if (status == SOCKET_ERROR)
		{
			m_log.log(Logger::LogLevel::error, "Failed to send message");
		}
	}
}

void Client::listenToMessageReceived(std::function<void(const std::string&)> callbackFunction)
{
	m_messageListener = callbackFunction;
}

bool Client::isConnected() const
{
	return m_connected;
}

void Client::disconnectFromServer()
{
	if (m_connected)
	{
		closesocket(m_connection);
		WSACleanup();

		m_connected = false;
		m_readThread.detach();

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
		if (m_messageListener)
		{
			m_messageListener(std::string(messageBuf));
		}
	}
}
