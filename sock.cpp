/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 01:11:53 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/12 02:06:56 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock.hpp"

Sock::Sock(int *ports, std::string ip_addr) : ip_addr(ip_addr)
{
	this->ports = ports;
	for (int i = 0; i < N_SERVERS; i++)
	{
		this->sock_ent[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (this->sock_ent[i] == -1)
		{
			std::cerr << "socket creation failed\n";
			exit(1);
		}
		if (fcntl(this->sock_ent[i], F_SETFL, O_NONBLOCK) == -1)
		{
			perror("fcntl");
			exit(1);
		}
		this->sock_addr[i].sin_family = AF_INET;
		this->sock_addr[i].sin_port = htons(ports[i]);
		if (inet_pton(AF_INET, ip_addr.c_str(), &this->sock_addr[i].sin_addr) <= 0) {
			std::cerr << "Invalid address/ Address not supported\n";
			exit(1);
		}	
	}
}

int	Sock::bind_sock()
{
	for (int i = 0; i < N_SERVERS; i++)
	{
		if (bind(this->sock_ent[i] ,(struct sockaddr *)&this->sock_addr[i], sizeof(this->sock_addr[i])) == -1)
		{
			std::cerr << "Failed to bind socket." << std::endl;
			exit(1);
		}
		std::cout << "socket bind " << this->ip_addr << ":" << htons(this->sock_addr[i].sin_port) << "\n";
	}
	for (int i = 0; i < N_SERVERS; i++)
	{
		this->fds[i].fd = this->sock_ent[i];
		this->fds[i].events = POLLIN;
	}
	for (int i = N_SERVERS; i < MAX_CLIENTS + N_SERVERS; i++)
		this->fds[i].fd = -1;
	return (0);
}

int	Sock::listen_sock()
{
	for (int i = 0; i< N_SERVERS; i++)
	{
		if (listen(sock_ent[i], SOMAXCONN) == -1)
		{
			std::cerr << "Failed to listen on socket\n";
			exit(1);
		}
	}
	for (int i = 0; i < N_SERVERS; i++)
	{
		std::cout << "listenning on port " << this->ports[i] << "\n";
	}
	return (0);
}

int	Sock::accept_sock(int server_fd)
{
	sockaddr_in client_addr;
	char ip[INET_ADDRSTRLEN];

	socklen_t client_size = sizeof(client_addr);
	int client_socket = accept(this->sock_ent[server_fd], (sockaddr*)&client_addr, &client_size);
	if (client_socket == -1)
	{
		std::cerr << "Failed to grab connection." << std::endl;
		return (-1);
	}
	inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);
	std::cout << "Client connected: " << ip << "\n";
	send(client_socket, "ACK!\n", 5, 0);
	for (int i = N_SERVERS; i < MAX_CLIENTS + N_SERVERS; i++)
	{
		if (this->fds[i].fd == -1)
		{
			this->fds[i].fd = client_socket;
			this->fds[i].events = POLLIN;
			break;
		}
	}

	return (client_socket);  // Return the client socket
}

void	Sock::recv_data(int client_sock)
{
	char buffer[1024];
	int valread = read(client_sock, buffer, 1024);
	if (valread == 0)
	{
		std::cout << "Client " << client_sock << " : " << " disconnected\n";
		close(client_sock);
		for (int i = N_SERVERS; i < MAX_CLIENTS + N_SERVERS; i++)
		{
			if (this->fds[i].fd == client_sock)
			{
				this->fds[i].fd = -1;
				break;
			}
		}
	}
	else
	{
		buffer[valread] = '\0';
		std::cout << "Client " << client_sock << " :\n" << buffer << "\n";
	}
}

void	Sock::close_sock()
{
	for (int i = 0; i < N_SERVERS; i++)
		close(this->sock_ent[i]);
	std::cout << "socket closed\n";
}

void	Sock::init_server()
{
	bind_sock();
	listen_sock();
	while (true)
	{
		// poll() will block until an event occurs even if the server socket is non blocking
		int	ret = poll(this->fds, MAX_CLIENTS + 1, -1);
		if (ret == -1)
		{
			std::cerr << "poll failed\n";
			for (int i = 0; i < N_SERVERS; i++)
				close(this->sock_ent[i]);
			exit(1);
		}
		for (int i = 0; i < MAX_CLIENTS + N_SERVERS; i++)
		{
			if (this->fds[i].revents & POLLIN)
			{
				if (i < N_SERVERS)
				{
					int client_sock = accept_sock(i);
					if (client_sock == -1)
					{
						std::cerr << "Failed to accept client\n";
						exit(1);
					}
				}
				else
				{
					recv_data(this->fds[i].fd);
				}
			}
		}
	}
	
}

Sock::~Sock()
{
	
}