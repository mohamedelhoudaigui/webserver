/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:48:03 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/16 05:19:30 by mel-houd         ###   ########.fr       */
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
		void	CGISetup(cgi_params& Params);
		void	Execute();
		
	private:
	   

		int				ProcId;
		const char		**env;
};


void	TestCGI();

#endif