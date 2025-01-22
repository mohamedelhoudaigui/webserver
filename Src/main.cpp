/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2025/01/22 11:19:32 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Config.hpp"
#include "../Include/CGI.hpp"


int main(int ac, char **av)
{
	// testing :
	// TestConfig();
	// TestCGI();

	if (ac != 2)
	{
		std::cout << "usage: ./webserv {ConfigFile}" << std::endl;
		return (1);
	}

	try {
		Config c(av[1]);
		c.Parse();

		SocketLayer server(c);
		server.Run();
	}
	catch (const std::exception& e)
	{
		Logger(FATAL, e.what());
		exit(1);
	}
}
