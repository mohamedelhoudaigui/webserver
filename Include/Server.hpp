/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:45:34 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/11 18:15:03 by mel-houd         ###   ########.fr       */
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
		//void	OpenSockets();

	private:
		std::vector<unsigned int>	SocketPorts;
};

class Server
{
	public:
		Server(unsigned int Port, unsigned int BufferSize, unsigned int MaxClients);
		void	BindServer();
		void	GetServerInfo();
		void	ListenServer();

		void	SelectSetup();
		void	ClientActivity(fd_set& Readfds);
		void	ServerActivity(fd_set& Readfds);

		void	CloseClient(int ClientFd, int ClientIndex);
		void	HandleClient(int ClientFd, int ClientIndex, std::string& ReqBuffer);

	private:
		int					ServerSocket;
		sockaddr_in			ServerAddrStruct;
		std::vector<int>	Clients;

		const unsigned int	Port;
		const unsigned int	BufferSize;
		const unsigned int	MaxClients;
};


#endif