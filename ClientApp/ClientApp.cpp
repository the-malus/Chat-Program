#include "Client.h"
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
	std::string name;
	std::cout << "Enter your name: ";
	std::getline(std::cin, name);

	Client c(name, Logger::LogLevel::all);
	c.connectToServer("127.0.0.1");
	return 0;
}