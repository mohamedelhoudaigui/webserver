/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:22:32 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/21 10:19:46 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/sock.hpp"

void	gen_server(std::string host, std::vector<int> ports, std::vector<server_config>& servers) // gen servers while there is no config file handling
{
	std::map<std::string, std::string>	routes;
	std::vector<std::string>	methods;

	routes["/"] = "./www/index.html";
	routes["/index.html"] = "./www/index.html";
	methods.push_back("get");
	methods.push_back("post");
	methods.push_back("delete");
	for (size_t i = 0; i < ports.size(); i++)
	{
		server_config server;
		server.port = ports[i];
		server.host = host;
		server.methods = methods;
		server.routes = routes;
		servers.push_back(server);
	}
}


int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "need port\n";
		exit(1);
	}
	std::vector<server_config>	servers;
	std::string	host =			"10.11.13.2";
	std::vector<int>			ports;

	ports.push_back(std::atoi(av[1]));
	gen_server(host, ports, servers);
	{
		Sock sockets(servers);
		sockets.init_server();
	}
    return 0;
}