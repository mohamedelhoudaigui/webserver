/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:21:24 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/09 03:40:25 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_HPP
#define SOCK_HPP

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

#define MAX_CLIENTS 30

class   Sock
{
	public:
		int         sock_ent;
		sockaddr_in sock_addr;
		std::string	ip_addr;
		size_t		port;
		struct pollfd fds[MAX_CLIENTS + 1];


		Sock(size_t port = 0, std::string ip_addr = "0.0.0.0");
		~Sock();
		int		bind_sock();
		int		listen_sock();
		int		accept_sock();
		void	close_sock();
		void	recv_data(int client_sock);
		void	init_server();
		
};



#endif