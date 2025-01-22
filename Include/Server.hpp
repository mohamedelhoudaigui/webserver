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
#include <cstring>

#include "Config.hpp"
#include "Client.hpp"
#include "Tools.hpp"


class SocketLayer
{
	public:
		SocketLayer(Config& c);
		int				OpenSocket(unsigned int Port);
		void			OpenServerSockets();
		int				BindSocket(int fd, int Port);
		int				SocketListen(int fd, int BufferSize, int Port);

		void			Run();
		~SocketLayer();

	private:
		std::vector<unsigned int>			ServerSockets;
		Config&								Conf;	
		
};


#endif
