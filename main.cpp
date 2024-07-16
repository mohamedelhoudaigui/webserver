/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:22:32 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/16 03:29:37 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/sock.hpp"

void	gen_server(std::string host, std::vector<int> ports, std::vector<server_config>& servers)
{
	for (size_t i = 0; i < ports.size(); i++)
	{
		server_config server;
		server.port = ports[i];
		server.host = host;
		servers.push_back(server);
	}
}


int main(void)
{
	std::vector<server_config>	servers;
	std::string	host = "127.0.0.1";
	std::vector<int>			ports;
	ports.push_back(9999);
	ports.push_back(9998);

	gen_server(host, ports, servers);

	Sock sockets(servers);
	sockets.init_server();
	sockets.close_sock();
    return 0;
}