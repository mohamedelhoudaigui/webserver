/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:50:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/12 00:09:49 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Server.hpp"

// Socket openning:


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
}


unsigned int	SocketLayer::OpenSocket(unsigned int Port)
{
	sockaddr_in	AddrServer;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	int flags = fcntl(fd, F_GETFL, 0);

    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	AddrServer.sin_family = AF_INET;
	AddrServer.sin_port = htons(Port);
	AddrServer.sin_addr.s_addr = INADDR_ANY;
	bind(fd, (struct sockaddr*)&AddrServer, sizeof(AddrServer));
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
			if (listen(fd, 100) >= 0)
			{
				std::cout << "Server listens on port : " << port->first << " fd : " << fd << std::endl;
			}
		}
	}
}

void	SocketLayer::SelectSetup()
{
	int		Activity;
	int		MaxFd;
	fd_set	Fds;

	FD_ZERO(&Fds); // refresh Fd set
	while (true)
	{

		for (int i = 0; i < ServerSockets.size(); ++i) // add all server sockets first
		{
			FD_SET(ServerSockets[i], &Fds);
		}
		MaxFd = ServerSockets.back();

		for (int i = 0; i < ClientSockets.size(); ++i)
		{
			FD_SET(ClientSockets[i], &Fds);
			if (ClientSockets[i] > MaxFd)
				MaxFd = ClientSockets[i];
		}
		
		Activity = select(MaxFd + 1, &Fds, NULL, NULL, NULL);
        if (Activity < 0)
            std::cerr << "Select error" << std::endl;

        ServerActivity(Fds);
		ClientActivity(Fds);
	}
}


void	SocketLayer::ServerActivity(fd_set& Fds)
{
	struct sockaddr_in	Address;
	int					NewSocket;
    int Addrlen = sizeof(Address);

	for (int i = 0; i < ServerSockets.size(); ++i)
	{
		if (FD_ISSET(ServerSockets[i], &Fds)) // activity from server socket
		{
			NewSocket = accept(ServerSockets[i], (struct sockaddr *)&Address, (socklen_t*)&Addrlen);
			ClientSockets.push_back(NewSocket);
		}
	}
}

static void	CloseClient(unsigned int fd, std::vector<unsigned int>& ClientSockets)
{
	std::vector<unsigned int>::iterator it = find(ClientSockets.begin(), ClientSockets.end(), fd);
	ClientSockets.erase(it);
}

void	SocketLayer::ClientActivity(fd_set& Fds)
{
	int		ClientFd;
	char	Buffer[10];
	int		Bytes;

	for (int i = 0; i < ClientSockets.size(); i++) // Else it's some IO operation on a client socket
		{
            ClientFd = ClientSockets[i];
            if (FD_ISSET(ClientFd, &Fds))
			{
                if ((Bytes = read(ClientFd, Buffer, 10)) == 0)
                	CloseClient(ClientFd, ClientSockets);
				else
				{
					Buffer[Bytes] = '\0';
					std::string	ReqBuffer(Buffer);
					std::cout << ReqBuffer << std::endl;
                	CloseClient(ClientFd, ClientSockets);
				}
            }
        }
}
