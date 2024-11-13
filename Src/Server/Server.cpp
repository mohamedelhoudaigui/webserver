/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:50:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/13 12:49:57 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Server.hpp"



// to support multiple servers to listen to the same port 
// we cant know what server the client want to connect to 
// since we working with INADDR_ANY so parsing the request
// should retrieve the server by the header host (or servernamew) if no server
// is defined the request should be dropped or support a default server

// the default size for listen queue is 1024 because we do not
// support the max clients directive

SocketLayer::SocketLayer(Config& c)
{
	ConfigFile res = c.GetResult();
	std::vector<ServerConf>::iterator Server;

	for (Server = res.servers.begin(); Server != res.servers.end(); ++Server)
	{
		std::vector<unsigned int>::iterator Port;
		for (Port = Server->Port.begin(); Port != Server->Port.end(); ++Port)
			this->SocketPorts[*Port] += 1;
	}
	
	this->LogFile.open("/Users/mel-houd/Desktop/webserver/Logs/ServerLog", std::ios::out | std::ios::trunc);
	if (!LogFile.is_open())
		std::cout << "log file not open !" << std::endl;
	this->Conf = c.GetResult();
}


unsigned int	SocketLayer::OpenSocket(unsigned int Port)
{
	sockaddr_in	AddrServer;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		this->LogFile << "failed to open socket into port " << Port << std::endl;
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
	{
		close(fd);
		this->LogFile << "failed to get socket flags on port " << Port << std::endl;
		return (-1);
	}

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		close(fd);
		this->LogFile << "failed to set socket flags to non-blocking on port " << Port << std::endl;
		return (-1);
	}

	AddrServer.sin_family = AF_INET;
	AddrServer.sin_port = htons(Port);
	AddrServer.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, (struct sockaddr*)&AddrServer, sizeof(AddrServer)) < 0)
	{
		close(fd);
		this->LogFile << "failed to bind socket on port " << Port << std::endl;
		return (-1);
	}

	return (fd);
}

void	SocketLayer::OpenServerSockets()
{
	std::map<unsigned int, int>::iterator port;
	
	for (port = this->SocketPorts.begin(); port != this->SocketPorts.end(); ++port)
	{
		int	fd = OpenSocket(port->first);
		if (fd > 0)
		{
			this->ServerSockets.push_back(fd);
			if (listen(fd, 1024) == 0)
			{
				this->LogFile << "Server listens on port : " << port->first << " fd : " << fd << std::endl;
			}
			else
			{
				this->LogFile << "failed to listen on port " << port->first << std::endl;
			}
		}
	}
}


void	SocketLayer::CloseClient(unsigned int ClientFd)
{
	close(ClientFd);
	std::vector<unsigned int>::iterator it = find(ClientSockets.begin(), ClientSockets.end(), ClientFd);
	ClientSockets.erase(it);
}

SocketLayer::~SocketLayer()
{
	for (int i = 0; i < ServerSockets.size(); ++i)
		close(ServerSockets[i]);
	for (int i = 0; i < ClientSockets.size(); ++i)
		close(ClientSockets[i]);
	this->LogFile.close();
}