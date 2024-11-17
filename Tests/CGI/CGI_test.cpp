/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 05:09:11 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/16 12:13:20 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Include/CGI.hpp"

void	TestCGI(char **env)
{
	CGI c;
	std::string	script = "/Users/mel-houd/Desktop/webserver/Tests/CGI/python_script.py";

	cgi_params p;
	p.PATH_INFO = script;
	p.HTTP_USER_AGENT = script;
	p.QUERY_STRING = script;
	p.REMOTE_ADDR = script;
	p.REQUEST_METHOD = script;
	p.SCRIPT_NAME = script;
	p.SERVER_NAME = script;
	p.SERVER_PORT = script;
	std::string	Request = "hello world\n";

	c.CGISetup(p, Request, env);

	std::cout << c.GetResponse() << std::endl;
}