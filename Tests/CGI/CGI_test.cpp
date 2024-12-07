/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 05:09:11 by mel-houd          #+#    #+#             */
/*   Updated: 2024/12/07 04:34:00 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Include/CGI.hpp"


void	TestCGI()
{
	std::fstream	a("./testLog");
	CGI				c;
	cgi_params		p;

	std::string	ScriptPath = "/Users/mel-houd/Desktop/webserver/Tests/CGI/python_script";
	std::string	ScriptName = "python_script";
	std::string	execBinary = "/bin/bash";

	p.PATH_INFO = ScriptPath;
	p.SCRIPT_NAME = ScriptName;
	p.HTTP_USER_AGENT = "test_user_agent";
	p.QUERY_STRING = "test_query_string";
	p.REMOTE_ADDR = "test_remote_addr";
	p.REQUEST_METHOD = "test_req_method";
	p.SERVER_NAME = "test_server_name";
	p.SERVER_PORT = "test_server_port";

	std::string	ReqBody = "hello world\n";

	c.CGISetup(p, ReqBody, execBinary);

	std::cout << c.GetResponse() << std::endl;
}