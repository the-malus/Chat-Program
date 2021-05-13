#include "BoostClient.h"
#include <iostream>

BoostClient::BoostClient(const std::string& name, LogLevel logLevel)
	: m_name(name), m_log(logLevel), m_ioContext(), m_socket(m_ioContext)
{
}

BoostClient::~BoostClient()
{
	if (m_ioContextThread)
	{
		m_ioContextThread->join();
		delete m_ioContextThread;
	}
}

void BoostClient::connectToServer(const std::string& serverAddress, const std::string& port)
{
	log(logInfo, "connecting");
	asio::ip::tcp::resolver resolver(m_ioContext);
	auto endpoints = resolver.resolve(serverAddress, port);

	asio::async_connect(m_socket, endpoints,
		[this](std::error_code error, asio::ip::tcp::endpoint)
		{
			if (!error)
			{
				log(logError, "failed to connect");
				readMessages();
			}
		});

	m_ioContextThread = new std::thread([this]() { m_ioContext.run(); });
}

void BoostClient::disconnectFromServer()
{
	asio::post(m_ioContext, [this]() { m_socket.close(); });
}

void BoostClient::sendMessage(const std::string& message)
{
	asio::post(m_ioContext,
		[this, message]()
		{
			asio::async_write(m_socket,
				asio::buffer(message.c_str(), message.length()+1),
				[this, message](std::error_code error, std::size_t)
				{
					if (error)
					{
						log(logError, "failed to send message");
						m_socket.close();
					}
					else
					{
						log(logMessage, message);
					}
				});
		});
}

void BoostClient::readMessages()
{
	char message[256];
	asio::async_read(m_socket,
		asio::buffer(message, 256),
		[this, message](std::error_code error, std::size_t)
		{
			if (error)
			{
				log(logError, "failed to read message");
				m_socket.close();
			}
			else
			{
				log(logMessage, std::string(message));
			}
		});
}

void BoostClient::log(LogLevel logLevel, const std::string& message) const
{
	if (logLevel < m_log) return;

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (logLevel)
	{
	case logWarning:
		SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	case logError:
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
	case logInfo:
	case logMessage:
	default:
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	std::cout << message << std::endl;
}
