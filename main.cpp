/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:22:32 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/26 11:53:13 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/Sock.hpp"
#include "./headers/Gen_dir.hpp"

void	gen_server(std::string host, std::vector<int> ports, std::vector<server_config>& servers) // gen servers while there is no config file handling
{
	std::map<std::string, std::string>	routes;
	std::vector<std::string>	methods;

	routes["/"] = "./www/index.html";
	routes["/styles.css"] = "./www/styles.css";
	routes["/img.png"] = "./www/img.png";
	routes["/index.html"] = "./www/index.html";
	routes["/script.js"] = "./www/script.js";
	routes["/audio.mp3"] = "./www/audio.mp3";

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

	Gen_dir	gg("www");
	gg.generate();
	// if (ac != 2)
	// {
	// 	std::cerr << "need port\n";
	// 	exit(1);
	// }
	// std::vector<server_config>	servers;
	// std::string	host =			"10.13.10.13";
	// std::vector<int>			ports;

	// ports.push_back(std::atoi(av[1]));
	// try
	// {
	// 	gen_server(host, ports, servers);
	// 	Sock sockets(servers);
	// 	sockets.init_server();
	// }
	// catch (...)
	// {
	// 	std::cerr << "An unknown error occurred\n";
	// 	exit(1);
	// }
    return 0;
}