/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:45:34 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/13 11:30:36 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>

#include "Config.hpp"
#include "Request.hpp"
#include "SelectObj.hpp"
#include "Client.hpp"


class SocketLayer
{
	public:
		SocketLayer(Config& c);
		unsigned int	OpenSocket(unsigned int Port);
		void			OpenServerSockets();
		void			SelectSetup();
		void			ClientActivity();
		void			ServerActivity();
		void			CloseClient(unsigned int ClientFd);
		~SocketLayer();

	private:
		std::map<unsigned int, int>			SocketPorts;
		std::vector<unsigned int>			ServerSockets;
		std::vector<unsigned int>			ClientSockets;
		std::fstream						LogFile;
		ConfigFile							Conf;
		SelectObj							Selector;
		std::map<int, Client>					Clients;		
		
};


#endif