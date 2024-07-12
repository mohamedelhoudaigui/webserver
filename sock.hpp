/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:21:24 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/12 02:07:47 by mel-houd         ###   ########.fr       */
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
#define N_SERVERS 2

class   Sock
{
	public:
		int         sock_ent[N_SERVERS];
		sockaddr_in sock_addr[N_SERVERS];
		std::string	ip_addr;
		int			*ports;
		struct pollfd fds[MAX_CLIENTS + N_SERVERS];


		Sock(int *ports, std::string ip_addr = "0.0.0.0");
		~Sock();
		int		bind_sock();
		int		listen_sock();
		int		accept_sock(int server_fd);
		void	close_sock();
		void	recv_data(int client_sock);
		void	init_server();
		
};



#endif