/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:45:34 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/14 11:37:48 by mel-houd         ###   ########.fr       */
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
#include "KqueueObj.hpp"
#include "Client.hpp"
#include "Helper.hpp"


class SocketLayer
{
	public:
		SocketLayer(Config& c);
		int				OpenSocket(unsigned int Port);
		void			OpenServerSockets();

		int				BindSocket(int fd, int Port);
		int				SocketListen(int fd, int BufferSize, int Port);

		void			RunKqueue();
		~SocketLayer();

	private:
		std::map<unsigned int, int>			SocketPorts; // map just for ignoring duplicated ports
		std::vector<unsigned int>			ServerSockets;
		std::fstream						LogFile;
		Config&								Conf;	
		
};


#endif