/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:22:32 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/13 09:58:12 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock.hpp"
#include "parse_config.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "args error\n";
		exit(1);
	}
	Parse_config	init_serv(av[1]);
	init_serv.parse();
	for (int i = 0; i < init_serv.servers.size(); i++)
	{
		std::cout << "server number " << i << " :\n";
		std::cout << "port : " << init_serv.servers[i].port << "\n";
		std::cout << "host : " << init_serv.servers[i].host << "\n";
		std::cout << "body size : " << init_serv.servers[i].body_size << "\n";
		for (const auto& pair : init_serv.servers[i].routes)
		{
			bool gg = true;
			std::cout << "route :" << pair.first << "\n";
			for (const auto& elem : init_serv.servers[i].routes[pair.first])
			{
				if (gg)
				{
					std::cout << "path : " << elem << "\n";
					gg = false;
				}
				else
				{
					std::cout << "method : " << elem << "\n";
				}
			}
		}
		for (const auto& pair : init_serv.servers[i].error_pages) {
			std::cout << pair.first << ": " << pair.second << std::endl;
		}
		std::cout << "\n";
	}

	// change ip address
	// int	ports[1] = {PORT};
    // Sock server(ports, IP);
	// server.init_server();
	// server.close_sock();
    return 0;
}