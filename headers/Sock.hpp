/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sock.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theworld27 <theworld27@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:21:24 by mel-houd          #+#    #+#             */
/*   Updated: 2024/09/02 08:55:20 by theworld27       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_HPP
#define SOCK_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Structes.hpp"
#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <map>
#include <cstdlib>

#define MAX_CLIENTS 100
#define MAX_PORT 65535
#define MIN_PORT 0
#define MIN_STATUS 100
#define MAX_STATUS 599
#define MAX_BODY_SIZE 10000000
#define READ_BUFFER 1024
#define LOG_FILE "log.txt"


class   Sock
{
	public:
		std::vector<int>			servers_socket;
		std::vector<sockaddr_in>	sock_addr;
		struct pollfd				fds[MAX_CLIENTS];
		std::vector<server_config>	servers;
		std::map<int, int>			clients;
		Logger						log_file;


		Sock(std::vector<server_config>& servers);
		~Sock();
		int		bind_sock();
		int		listen_sock();
		int		accept_sock(int server_fd);
		void	termiante_server();
		void	purge_client(int client_fd); // implement
		void	recv_data(int client_sock, server_config server);
		void	init_server();
		void	handle_req(char *buffer, int client_sock, server_config server);
		void	put_client(int client_fd, char *ip);
		
};



#endif