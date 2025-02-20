#include "../../Include/Server.hpp"


SocketLayer::SocketLayer(Config& c): Conf(c)
{
	Logger(INFO, "starting to open sockets");
	ConfigFile res = Conf.GetResult();
	const std::vector<ServerConf> Servers = res.GetServers();
	std::vector<ServerConf>::const_iterator Server;

	for (Server = Servers.begin(); Server != Servers.end(); ++Server)
	{
		int	fd = OpenSocket(Server->GetPort());
		std::stringstream ss;
		ss << Server->GetPort();
		if (fd > 0)
			this->ServerSockets.push_back(fd);
		else
			Logger(WARNING, "failed to listen on port " + ss.str());
	}
}

int	SocketLayer::OpenSocket(unsigned int Port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		return (-1);
	if (SetNonBlocking(fd) == -1)
		return (-1);
	if (BindSocket(fd, Port) == -1)
		return (-1);
	if (listen(fd, 5) == -1)
		return (-1);
	else
	{
		std::stringstream ss;
		ss << Port;
		Logger(INFO, "Server listening on port " + ss.str());
	}
	return (fd);
}

int	SocketLayer::BindSocket(int fd, int Port)
{
	sockaddr_in	AddrServer;

	memset(&AddrServer, 0, sizeof(AddrServer));
	AddrServer.sin_family = AF_INET;
	AddrServer.sin_port = htons(Port);
	AddrServer.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, (struct sockaddr*)&AddrServer, sizeof(AddrServer)) < 0)
	{
		close(fd);
		return (-1);
	}
	return (0);
}


void	SocketLayer::Run()
{
	Poller	poller(this->ServerSockets, Conf);
	poller.Run();
}

SocketLayer::~SocketLayer()
{}