/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:50:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/14 18:39:35 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Server.hpp"

// server bind + listen setup :

Server::Server(unsigned int Port, unsigned int BufferSize, unsigned int MaxClients):
	Port(Port),
	BufferSize(BufferSize),
	MaxClients(MaxClients),
	Clients(MaxClients, 0)
{}

void	Server::BindServer() {
	this->ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->ServerSocket == -1)
	{
		std::cerr << "Failed to start server (socket init error)" << std::endl;
		exit(1);
	}

	int flags = fcntl(this->ServerSocket, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Error getting socket flags" << std::endl;
        exit(1);
    }
    if (fcntl(this->ServerSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Error setting socket to non-blocking mode" << std::endl;
        exit(1);
    }

	this->ServerAddrStruct.sin_family = AF_INET;
	this->ServerAddrStruct.sin_port = htons(this->Port);
	this->ServerAddrStruct.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->ServerSocket, (struct sockaddr*)&ServerAddrStruct, sizeof(ServerAddrStruct)) == -1)
	{
		std::cerr << "Error binding server socket" << std::endl;
		exit(1);
	}
}

void Server::GetServerInfo()
{
	struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(this->ServerSocket, (struct sockaddr*)&addr, &addr_len) == -1) {
        std::cerr << "Error getting socket name" << std::endl;
        exit(1);
    }
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
    std::cout << "Server is bound to IP: " << ip << " and port: " << ntohs(addr.sin_port) << std::endl;
}

void	Server::ListenServer() {
	if (listen(this->ServerSocket, 100) == -1) {
        std::cerr << "Error listening on server socket" << std::endl;
        exit(1);
	}
}

// select() code :

void	Server::SelectSetup()
{
	int		Activity, MaxFd;
    fd_set	Readfds;

	while(true)
	{
        FD_ZERO(&Readfds);
        FD_SET(ServerSocket, &Readfds);
        MaxFd = ServerSocket;

        for (int i = 0; i < MaxClients; i++) // add clients to Readfds struct
		{
            if(Clients[i] > 0)
                FD_SET(Clients[i], &Readfds);

            if(Clients[i] > MaxFd)
                MaxFd = Clients[i];
        }

        Activity = select(MaxFd + 1, &Readfds, NULL, NULL, NULL);
        if (Activity < 0)
            std::cerr << "Select error" << std::endl;

        ServerActivity(Readfds);
		ClientActivity(Readfds);
    }	
}

void	Server::ServerActivity(fd_set& Readfds)
{
	struct sockaddr_in	Address;
    int Addrlen =		sizeof(Address);
	int	NewSocket;

	if (FD_ISSET(ServerSocket, &Readfds)) // activity from server socket
	{
		NewSocket = accept(ServerSocket, (struct sockaddr *)&Address, (socklen_t*)&Addrlen);
		if (NewSocket < 0)
			std::cerr << "Accept failed" << std::endl;

		//std::cout << "New connection, socket fd is " << NewSocket 
		//          << ", ip is : " << inet_ntoa(Address.sin_addr) 
		//          << ", port : " << ntohs(Address.sin_port) << '\n';


		for (int i = 0; i < MaxClients; i++)
		{
			if (Clients[i] == 0) 
			{
				Clients[i] = NewSocket;
				break;
			}
		}
	}
}

// Client handeling :

void	Server::CloseClient(int ClientFd, int ClientIndex)
{
	close(ClientFd);
	this->Clients[ClientIndex] = 0;
}

void	Server::HandleClient(int ClientFd, int ClientIndex, std::string& ReqBuffer, int Valread) // req init here
{
	Request	r(ReqBuffer);
	r.Parse();
	std::cout << r;
	
	//send(sd, buffer, strlen(buffer), 0);
	CloseClient(ClientFd, ClientIndex);
	
}

void	Server::ClientActivity(fd_set& Readfds)
{
	int		ClientFd;
	char	Buffer[BufferSize];
	int		Valread;

	for (int i = 0; i < MaxClients; i++) // Else it's some IO operation on a client socket
		{
            ClientFd = Clients[i];
            if (FD_ISSET(ClientFd, &Readfds))
			{
                if ((Valread = read(ClientFd, Buffer, BufferSize)) == 0)
                	CloseClient(ClientFd, i);
				else
				{
					Buffer[Valread] = '\0';
					std::string	ReqBuffer(Buffer);
					HandleClient(ClientFd, i, ReqBuffer, Valread);
				}
            }
        }
}
