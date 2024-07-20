/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:21:24 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/20 06:41:20 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_HPP
#define SOCK_HPP

#include "../request_response/Request.hpp"
#include "../request_response/Response.hpp"
#include "../structes.hpp"
#include "./Logger.hpp"

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