#include "../Include/Server.hpp"
#include "../Include/Config.hpp"
#include "../Include/CGI.hpp"


int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "usage: ./webserv {ConfigFile}" << std::endl;
		return (1);
	}

	try {
		Config c(av[1]);
		c.Parse();

		SocketLayer server(c);
		server.Run();
	}
	catch (const std::exception& e)
	{
		Logger(FATAL, e.what());
	}
}