/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/12 00:38:21 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"


void	LaunchServer(int Port) {
	try 
	{
		Server	MainServer(Port, 16384, 500); // BufferSize - MaxClients
		MainServer.BindServer();
		MainServer.GetServerInfo();
		MainServer.ListenServer();
		MainServer.SelectSetup();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		exit(1);
	}
}


int main(int ac, char **av) {
	if (ac == 2)
	{
		LaunchServer(atoi(av[1]));
	}
}