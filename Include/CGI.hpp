#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fstream>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>

#include "HttpRequest.hpp"
#include "Types.hpp"
#include "Tools.hpp"
/*from CGI rfc:*/
/*The server acts as an application gateway.  It receives the request*/
/*from the client, selects a CGI script to handle the request, converts*/
/*the client request to a CGI request, executes the script and converts*/
/*the CGI response into a response for the client.*/

class   CGI
{
	public:
		CGI();
		~CGI();
		void						CGISetup(Request& request);
        void						Execute(Request& request);

        int                         GetStdoutFd(); // use this to inject cgi into epoll
		std::string&				GetResponse(); // cgi output here
		
	private:
		int							stdin_pipe[2];
		int							stdout_pipe[2];
		std::string					CgiResponse;

		std::vector<std::string>	PrepareEnv(Request& request);
        std::string                 convert_header_name(const std::string& header_name);

		void						WritePipe(int pipe_fd, const std::string& s);
		void						ReadPipe(int pipe_fd, std::string& s);
};


void	TestCGI(char **av);

