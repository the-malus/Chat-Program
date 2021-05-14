#include "ClientWindow.h"

#include <Client.h>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ClientWindow clientWindow(new Client());
	clientWindow.show();
	return a.exec();
}