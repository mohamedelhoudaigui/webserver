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
	Logger(INFO, "starting to open sockets");
	ConfigFile res = Conf.GetResult();
	std::vector<ServerConf>::iterator Server;

	for (Server = res.servers.begin(); Server != res.servers.end(); ++Server)
	{
		int	fd = OpenSocket(Server->Port);
		std::stringstream ss;
		ss << Server->Port;
		if (fd > 0)
		{
			Logger(DEBUG, "socket open in port " + ss.str());
			this->ServerSockets.push_back(fd);
		}
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
}


SocketLayer::~SocketLayer()
{}
