#include "Client.h"
#include <thread>
#include <chrono>
#include <iostream>

#include "ClientWindow.h"
//#include "ClientLogin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	std::string name;
	std::cout << "Enter your name: ";
	std::getline(std::cin, name);
	Client* c = new Client(name, Logger::LogLevel::all);

	std::string server;
	std::cout << "Enter server IP (blank for localhost): ";
	std::getline(std::cin, server);
	if (server.empty())
	{
		server = "127.0.0.1";
	}
	c->connectToServer(server);

	QApplication a(argc, argv);
	//a.setQuitOnLastWindowClosed(false);

	//ClientLogin clientLogin;
	ClientWindow clientWindow(c);
	clientWindow.show();
	//QObject::connect(&clientLogin, &ClientLogin::windowClosed, &clientWindow, &ClientWindow::show);
	//clientLogin.show();

	//c.connectToServer("127.0.0.1");
	return a.exec();
}