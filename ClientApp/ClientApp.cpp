#include "Client.h"
#include "BoostClient.h"
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
	Client c("clientName");
	c.connectToServer("127.0.0.1");
	//c.sendMessage("hi from clientName!");
	//std::this_thread::sleep_for(std::chrono::seconds(3));
	//c.sendMessage("a new message?");
	//std::this_thread::sleep_for(std::chrono::seconds(3));
	//c.disconnectFromServer();
	return 0;
}