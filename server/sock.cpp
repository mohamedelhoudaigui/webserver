/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 01:11:53 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/20 08:26:08 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock.hpp"

Sock::Sock(std::vector<server_config>& servers) : servers(servers), log_file(LOG_FILE)
{
	this->sock_addr.resize(servers.size());
	this->servers_socket.resize(servers.size());
	for (unsigned long i = 0; i < servers.size(); i++)
	{
		this->servers_socket[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (this->servers_socket[i] == -1)
		{
			std::cerr << "socket creation failed\n";
			exit(1);
		}
		if (fcntl(this->servers_socket[i], F_SETFL, O_NONBLOCK) == -1)
		{
			perror("fcntl");
			exit(1);
		}
		this->sock_addr[i].sin_family = AF_INET;
		this->sock_addr[i].sin_port = htons(servers[i].port);
		if (inet_pton(AF_INET, servers[i].host.c_str(), &this->sock_addr[i].sin_addr) <= 0)
		{
			std::cerr << "Invalid address/ Address not supported\n";
			exit(1);
		}	
	}
}

void	Sock::purge_client(int client_fd)
{
	close(client_fd);
	for (int i = servers.size(); i < MAX_CLIENTS; i++)
	{
		if (fds[i].fd == client_fd)
			this->fds[i].fd = -1;
	}
	std::map<int, int>::iterator it = this->clients.find(client_fd);
    if (it != this->clients.end())
    	this->clients.erase(it);
}

void	Sock::handle_req(char *buffer, int client_sock, server_config server)
{
	Request req(buffer, server);
	req.parse_req();
	if (!req.valid_req)
	{
		this->log_file.write("Invalid request, code = " + req.status + "\n");
		this->log_file.write(req.request);
	}
	else
		this->log_file.write(req.request);
	Response	res(req);
	this->log_file.write(res.res + "\n");
	send(client_sock, res.res.c_str(), res.res.size(), 0);
	purge_client(client_sock);
}

void	Sock::put_client(int client_fd, char *ip)
{
	for (unsigned long i = servers.size(); i < MAX_CLIENTS; i++)
	{
		if (this->fds[i].fd == -1)
		{
			this->fds[i].fd = client_fd;
			this->fds[i].events = POLLIN;
			this->log_file.write("Client connected: " + ctos(ip) + "\n");
			return ;
		}
	}
	this->log_file.write("no place for client in fds array\n");
}

void	Sock::termiante_server()
{
	for (unsigned long i = 0; i < servers_socket.size(); i++)
		close(this->servers_socket[i]);
	for (std::map<int, int>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
		close(it->first);
	this->log_file.write("server terminated\n");
}

int	Sock::bind_sock()
{
	for (unsigned long i = 0; i < servers.size(); i++)
	{
		if (bind(this->servers_socket[i] ,(struct sockaddr *)&this->sock_addr[i], sizeof(this->sock_addr[i])) == -1)
		{
			this->log_file.write("Failed to bind server socket number " + itos(i) + "\n");
			exit(1);
		}
		this->log_file.write("socket bind " + this->servers[i].host + ":" + itos(htons(this->sock_addr[i].sin_port)) + "\n");
	}
	for (unsigned long i = 0; i < servers.size(); i++)
	{
		// add server fd fo server_config structe
		this->servers[i].fd = this->servers_socket[i];
		this->fds[i].fd = this->servers_socket[i];
		this->fds[i].events = POLLIN;
	}
	for (int i = servers.size(); i < MAX_CLIENTS; i++)
		this->fds[i].fd = -1;
	return (0);
}

int	Sock::listen_sock()
{
	for (unsigned long i = 0; i< servers.size(); i++)
	{
		if (listen(servers_socket[i], SOMAXCONN) == -1)
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
	int client_socket = accept(this->servers_socket[server_fd], (sockaddr*)&client_addr, &client_size);
	if (client_socket == -1)
	{
		this->log_file.write("Failed to grab connection\n");
		return (-1);
	}
	inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);
	put_client(client_socket, ip);
	return (client_socket);  // Return the client socket
}

void	Sock::recv_data(int client_sock, server_config server)
{
	char buffer[READ_BUFFER];
	int valread = read(client_sock, buffer, 1024);
	buffer[valread] = '\0';
	if (valread == 0)
	{
		this->log_file.write("Client " + itos(client_sock) + " : " + " disconnected\n");
		purge_client(client_sock);
	}
	else // handle req, res and log to a file
		handle_req(buffer, client_sock, server);
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
			this->log_file.write("poll failed\n");
			termiante_server();
			exit(1);
		}
		for (unsigned long i = 0; i < MAX_CLIENTS; i++)
		{
			if (this->fds[i].revents & POLLIN) // loop over all fd (servers and clients)
			{
				if (i < servers.size()) // if fd is a server
				{
					int client_sock = accept_sock(i);
					if (client_sock != -1)
						this->clients[client_sock] = i;
				}
				else // if fd is a client
					recv_data(fds[i].fd, this->servers[clients[fds[i].fd]]);
			}
		}
	}
	
}

Sock::~Sock()
{
	termiante_server();
}