/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:45:34 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/11 16:48:33 by mel-houd         ###   ########.fr       */
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


class Server
{
	public:
		Server(unsigned int Port, unsigned int BufferSize, unsigned int MaxClients);
		void	BindServer(); // 1
		void	GetServerInfo(); // 2
		void	ListenServer(); // 3
		void	SelectSetup(); // 4
		void	HandleClient(int ClientFd, int ClientIndex, char* Buffer, int Valread);

		void	CloseClient(int ClientFd, int ClientIndex);

	private:
		std::string			Addr;
		int					ServerSocket;
		sockaddr_in			ServerAddrStruct;
		std::vector<int>	Clients;

		const unsigned int	Port;
		const unsigned int	BufferSize;
		const unsigned int	MaxClients;
};


#endif