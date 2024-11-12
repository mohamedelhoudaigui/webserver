/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:45:34 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/11 23:49:00 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>

#include "Config.hpp"
#include "Request.hpp"


class SocketLayer
{
	public:
		SocketLayer(Config& c);
		unsigned int	OpenSocket(unsigned int Port);
		void			OpenServerSockets();
		void			SelectSetup();
		void			ClientActivity(fd_set& Fds);
		void			ServerActivity(fd_set& Fds);

	private:
		std::map<unsigned int, int>	SocketPorts;
		std::vector<unsigned int>	ServerSockets;
		std::vector<unsigned int>	ClientSockets;
};


#endif