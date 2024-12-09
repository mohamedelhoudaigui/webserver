/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:48:03 by mel-houd          #+#    #+#             */
/*   Updated: 2024/12/09 20:19:35 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

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
#include "HttpRequest.hpp"

class   CGI
{
	public:
		CGI();
		~CGI();
		void						CGISetup(HttpRequest &request);

		std::string&				GetResponse();
		std::string&				GetError();
		
	private:
		pid_t						ProcId;

		int							stdin_pipe[2];
		int							stderr_pipe[2];
		int							stdout_pipe[2];
		std::string					Response;
		std::string					Error;

		void						Execute(HttpRequest &request);
		std::vector<std::string>			PrepareEnv(HttpRequest &request);
		void						WritePipe(int pipe_fd, const std::string& s);
		void						ReadPipe(int pipe_fd, std::string& s);
};


void	TestCGI();

#endif