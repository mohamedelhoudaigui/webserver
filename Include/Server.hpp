#pragma once

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
#include "Poller.hpp"
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

