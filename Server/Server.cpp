/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:50:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/11 16:48:03 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"


Server::Server(unsigned int Port, unsigned int BufferSize, unsigned int MaxClients):
	Port(Port),
	BufferSize(BufferSize),
	MaxClients(MaxClients),
	Clients(MaxClients, 0)
{
}

void	Server::BindServer() {
	this->ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->ServerSocket == -1)
	{
		std::cerr << "Failed to start server (socket init error)\n";
		exit(1);
	}

	this->ServerAddrStruct.sin_family = AF_INET;
	this->ServerAddrStruct.sin_port = htons(this->Port);
	this->ServerAddrStruct.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->ServerSocket, (struct sockaddr*)&ServerAddrStruct, sizeof(ServerAddrStruct)) == -1)
	{
		std::cerr << "Error binding server socket\n";
		exit(1);
	}
}

void Server::GetServerInfo() {
	struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(this->ServerSocket, (struct sockaddr*)&addr, &addr_len) == -1) {
        std::cerr << "Error getting socket name\n";
        exit(1);
    }
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
    std::cout << "Server is bound to IP: " << ip << " and port: " << ntohs(addr.sin_port) << "\n";
}

void	Server::ListenServer() {
	if (listen(this->ServerSocket, 100) == -1) {
        std::cerr << "Error listening on server socket\n";
        exit(1);
	}
}

void	Server::HandleClient(int ClientFd, int ClientIndex, char* Buffer, int Valread) {
	Buffer[Valread] = '\0';
	std::cout << Buffer << '\n';
	//send(sd, buffer, strlen(buffer), 0);
	CloseClient(ClientFd, ClientIndex);
	
}

void	Server::SelectSetup()
{
	int					NewSocket, Activity, Valread, ClientFd, MaxFd;
    struct sockaddr_in	Address;
    char				Buffer[BufferSize];
    fd_set				Readfds;
    int Addrlen =		sizeof(Address);

	while(true)
	{
        FD_ZERO(&Readfds);
        FD_SET(ServerSocket, &Readfds);
        MaxFd = ServerSocket;

        for (int i = 0; i < MaxClients; i++)
		{
            ClientFd = Clients[i];

            if(ClientFd > 0)
                FD_SET(ClientFd, &Readfds);

            if(ClientFd > MaxFd)
                MaxFd = ClientFd;
        }

        Activity = select(MaxFd + 1, &Readfds, NULL, NULL, NULL);

        if (Activity < 0)
            std::cerr << "Select error" << '\n';

        if (FD_ISSET(ServerSocket, &Readfds)) // activity from server socket
		{
            if ((NewSocket = accept(ServerSocket, (struct sockaddr *)&Address, (socklen_t*)&Addrlen)) < 0)
                std::cerr << "Accept failed" << '\n';

            std::cout << "New connection, socket fd is " << NewSocket 
                      << ", ip is : " << inet_ntoa(Address.sin_addr) 
                      << ", port : " << ntohs(Address.sin_port) << '\n';


            for (int i = 0; i < MaxClients; i++)
			{
                if (Clients[i] == 0) 
				{
                    Clients[i] = NewSocket;
                    std::cout << "Adding to list of sockets as " << i << '\n';
                    break;
                }
            }
        }

        for (int i = 0; i < MaxClients; i++) // Else it's some IO operation on a client socket
		{
            ClientFd = Clients[i];
            if (FD_ISSET(ClientFd, &Readfds))
			{
                if ((Valread = read(ClientFd, Buffer, BufferSize)) == 0)
                	CloseClient(ClientFd, i);
				else      
					HandleClient(ClientFd, i, Buffer, Valread);
            }
        }
    }	
}


void	Server::CloseClient(int ClientFd, int ClientIndex)
{
	close(ClientFd);
	this->Clients[ClientIndex] = 0;
}