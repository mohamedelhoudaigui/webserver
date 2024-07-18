/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 01:11:53 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/18 23:21:45 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock.hpp"


Sock::Sock(std::vector<server_config>& servers) : servers(servers)
{
	this->sock_addr.resize(servers.size());
	this->sock_ent.resize(servers.size());
	for (unsigned long i = 0; i < servers.size(); i++)
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
		this->sock_addr[i].sin_port = htons(servers[i].port);
		if (inet_pton(AF_INET, servers[i].host.c_str(), &this->sock_addr[i].sin_addr) <= 0) {
			std::cerr << "Invalid address/ Address not supported\n";
			exit(1);
		}	
	}
}

int	Sock::bind_sock()
{
	for (unsigned long i = 0; i < servers.size(); i++)
	{
		if (bind(this->sock_ent[i] ,(struct sockaddr *)&this->sock_addr[i], sizeof(this->sock_addr[i])) == -1)
		{
			std::cerr << "Failed to bind socket." << std::endl;
			exit(1);
		}
		std::cout << "socket bind " << this->servers[i].host << ":" << htons(this->sock_addr[i].sin_port) << "\n";
	}
	for (unsigned long i = 0; i < servers.size(); i++)
	{
		this->fds[i].fd = this->sock_ent[i];
		this->fds[i].events = POLLIN;
	}
	// check size of fds vector !!
	for (int i = servers.size(); i < MAX_CLIENTS; i++)
		this->fds[i].fd = -1;
	return (0);
}

int	Sock::listen_sock()
{
	for (unsigned long i = 0; i< servers.size(); i++)
	{
		if (listen(sock_ent[i], SOMAXCONN) == -1)
		{
			std::cerr << "Failed to listen on socket\n";
			exit(1);
		}
	}
	for (unsigned long i = 0; i < servers.size(); i++)
	{
		std::cout << "listenning on port " << this->servers[i].port << "\n";
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
	// send(client_socket, "ACK!\n", 5, 0);
	for (unsigned long i = servers.size(); i < MAX_CLIENTS; i++)
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
		for (int i = servers.size(); i < MAX_CLIENTS; i++)
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
		Request req(buffer);
		this->requests.push(req);
		Response	res(req);
		send(client_sock, res.res.c_str(), res.res.size(), 0);
		// std::cout << res.res_buffer << "\n";
		// req.print_req();
		
	}
}

void	Sock::close_sock()
{
	for (unsigned long i = 0; i < servers.size(); i++)
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
		int	ret = poll(this->fds, MAX_CLIENTS, -1);
		if (ret == -1)
		{
			std::cerr << "poll failed\n";
			for (unsigned long i = 0; i < servers.size(); i++)
				close(this->sock_ent[i]);
			exit(1);
		}
		for (unsigned long i = 0; i < MAX_CLIENTS; i++)
		{
			if (this->fds[i].revents & POLLIN)
			{
				if (i < servers.size())
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