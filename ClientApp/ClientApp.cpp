#include "Client.h"
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
	Client c("clientName", Logger::LogLevel::all);
	c.connectToServer("127.0.0.1");
	return 0;
}