/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2025/01/26 14:48:56 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../Include/Server.hpp"
#include "../Include/Config.hpp"
// #include "../Include/CGI.hpp"


void	print_all_config(ConfigFile config) {
	const std::vector<ServerConf>& servers = config.GetServers();
	for (size_t i = 0; i < servers.size(); ++i)
	{
		std::cout << "Server: " << std::endl;
		std::cout << "    host = " << servers[i].GetHost() << std::endl;
		std::cout << "    port = " << servers[i].GetPort() << std::endl;
		std::cout << "    server name = " << servers[i].GetServerName() << std::endl;
		std::cout << "    root = " << servers[i].GetRoot() << std::endl;
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "usage: ./webserv {ConfigFile}" << std::endl;
		return (1);
	}

	Config c(av[1]);
	c.Parse();
	print_all_config(c.GetResult());
}
