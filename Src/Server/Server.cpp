/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:50:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/14 11:44:51 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Server.hpp"

SocketLayer::SocketLayer(Config& c): Conf(c)
{
	ConfigFile res = Conf.GetResult();
	std::vector<ServerConf>::iterator Server;

	for (Server = res.servers.begin(); Server != res.servers.end(); ++Server)
	{
		std::vector<unsigned int>::iterator Port;
		for (Port = Server->Port.begin(); Port != Server->Port.end(); ++Port) //map stores ports without dups
			this->SocketPorts[*Port] += 1;
	}

	this->LogFile.open("/Users/mel-houd/Desktop/webserver/Logs/ServerLog", std::ios::out | std::ios::trunc);
	if (!LogFile.is_open())
		std::cout << "log file not open !" << std::endl;
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
		this->LogFile << "failed to bind socket on port " << Port << std::endl;
		return (-1);
	}
	return (0);
}


int	SocketLayer::OpenSocket(unsigned int Port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		this->LogFile << "failed to open socket into port " << Port << std::endl;
		return (-1);
	}
	if (SetNonBlocking(fd, LogFile) == -1)
		return (-1);
	if (BindSocket(fd, Port) == -1)
		return (-1);
	return (fd);
}


int	SocketLayer::SocketListen(int fd, int BufferSize, int Port)
{
	if (listen(fd, 1024) == 0)
		this->LogFile << "Server listens on port : " << Port << " fd : " << fd << std::endl;
	else
	{
		this->LogFile << "failed to listen on port " << Port << std::endl;
		return (-1);
	}
	return (0);
}

void	SocketLayer::OpenServerSockets()
{
	std::map<unsigned int, int>::iterator port;
	
	for (port = this->SocketPorts.begin(); port != this->SocketPorts.end(); ++port)
	{
		int	fd = OpenSocket(port->first);
		if (fd > 0)
		{
			if (SocketListen(fd, 1024, port->first) == 0)
				this->ServerSockets.push_back(fd);
		}
	}
}

void	SocketLayer::RunKqueue()
{
	KqueueObj	handler(LogFile, ServerSockets, Conf);
	handler.Init();
	handler.AddServers();
	handler.Run(SetNonBlocking);
}


SocketLayer::~SocketLayer()
{
	this->LogFile.close();
}