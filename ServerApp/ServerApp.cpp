#include <Server.h>

int main()
{
	Server s(Logger::LogLevel::all);
	s.run();

	return 0;
}