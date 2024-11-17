/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/17 16:11:53 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Config.hpp"
#include "../Include/CGI.hpp"



int main(int ac, char **av ,char **env)
{
	// testing :
	//TestConfig();
	//TestCGI(env);

	if (ac != 2)
	{
		std::cout << "usage: ./webserv {ConfigFile}" << std::endl;
		return (1);
	}

	try {
		Config c(av[1]);
		c.Parse();

		CGI cgi;

		SocketLayer server(c);
		server.OpenServerSockets();
		server.RunKqueue();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;	
	}
}