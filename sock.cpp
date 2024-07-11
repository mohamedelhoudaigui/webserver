/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 01:11:53 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/09 23:11:51 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock.hpp"

Sock::Sock(size_t port, std::string ip_addr) : ip_addr(ip_addr), port(port)
{
	int	flags;

	this->sock_ent = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock_ent == -1)
	{
		std::cerr << "socket creation failed\n";
		exit(1);
	}
	flags = fcntl(this->sock_ent, F_GETFL, 0);
	if (flags == -1)
	{
		perror("fcntl");
		exit(1);
	}
	flags |= O_NONBLOCK;
	if (fcntl(this->sock_ent, F_SETFL, flags) == -1)
	{
		perror("fcntl");
		exit(1);
	}
	this->sock_addr.sin_family = AF_INET;
	this->sock_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip_addr.c_str(), &this->sock_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address/ Address not supported\n";
		exit(1);
	}
}

int	Sock::bind_sock()
{
	if (bind(this->sock_ent ,(sockaddr*)&this->sock_addr, sizeof(this->sock_addr)) == -1)
	{
		std::cerr << "Failed to bind socket." << std::endl;
		exit(1);
	}
	std::cout << "socket bind " << this->ip_addr << ":" << htons(this->sock_addr.sin_port) << "\n";
	this->fds[0].fd = this->sock_ent;
	this->fds[0].events = POLLIN;
	for (int i = 1; i < MAX_CLIENTS + 1; i++)
		this->fds[i].fd = -1;
	return (0);
}

int	Sock::listen_sock()
{
	if (listen(sock_ent, SOMAXCONN) == -1)
	{
		std::cerr << "Failed to listen on socket\n";
		exit(1);
	}
	std::cout << "listenning ...\n";
	return (0);
}

int	Sock::accept_sock()
{
	sockaddr_in client_addr;
	char ip[INET_ADDRSTRLEN];

	socklen_t client_size = sizeof(client_addr);
	int client_socket = accept(this->sock_ent, (sockaddr*)&client_addr, &client_size);
	if (client_socket == -1)
	{
		std::cerr << "Failed to grab connection." << std::endl;
		return (-1);
	}
	inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);
	std::cout << "Client connected: " << ip << "\n";
	send(client_socket, "ACK!\n", 5, 0);
	for (int i = 1; i < MAX_CLIENTS + 1; i++)
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
		for (int i = 1; i < MAX_CLIENTS + 1; i++)
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
		std::cout << "Client " << client_sock << " : " << buffer << "\n";
	}
}

void	Sock::close_sock()
{
	close(this->sock_ent);
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
			close(this->sock_ent);
			exit(1);
		}
		for (int i = 0; i < MAX_CLIENTS + 1; i++)
		{
			if (this->fds[i].revents & POLLIN)
			{
				if (i == 0)
				{
					int client_sock = accept_sock();
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