/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:21:24 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/18 22:59:06 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_HPP
#define SOCK_HPP

#include "../request_response/Request.hpp"
#include "../request_response/Response.hpp"

#include <iostream>
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
#include <queue>

#define MAX_CLIENTS 100
#define MAX_PORT 65535
#define MIN_PORT 0
#define MIN_STATUS 100
#define MAX_STATUS 599
#define MAX_BODY_SIZE 10000000

typedef	struct
{
	std::string							host;
	int									port;
}		server_config;


class   Sock
{
	public:
		std::vector<int>			sock_ent;
		std::vector<sockaddr_in>	sock_addr;
		struct pollfd				fds[MAX_CLIENTS];
		std::queue<Request>		requests;
		std::vector<server_config>	servers;


		Sock(std::vector<server_config>& servers);
		~Sock();
		int		bind_sock();
		int		listen_sock();
		int		accept_sock(int server_fd);
		void	close_sock();
		void	recv_data(int client_sock);
		void	init_server();
		
};



#endif