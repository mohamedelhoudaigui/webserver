/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:48:03 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/17 14:55:54 by mel-houd         ###   ########.fr       */
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

typedef struct cgi_params
{
	std::string SERVER_NAME;
	std::string SERVER_PORT; // server

	std::string REMOTE_ADDR; //client
	std::string HTTP_USER_AGENT;

	std::string REQUEST_METHOD; // request
	std::string QUERY_STRING;

	std::string PATH_INFO; // script info
	std::string SCRIPT_NAME;

}   cgi_params;


class   CGI
{
	public:
		CGI();
		~CGI();
		void	CGISetup(cgi_params& Params, std::string Request, char **e);

		std::string&	GetResponse();
		std::string&	GetError();
		
	private:
		pid_t			ProcId;
		char			**server_env;
		std::string		Request;
		cgi_params		Params;

		int				stdin_pipe[2];
		int				stdout_pipe[2];
		int				stderr_pipe[2];

		std::string		Response;
		std::string		Error;

		void	Execute();
		void	ReadPipe(int pipe_fd, std::string& s);
		void	WritePipe(int pipe_fd, std::string& s);
		char**	MakeEnv();
};


void	TestCGI(char **env);

#endif