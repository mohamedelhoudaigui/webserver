/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/11 22:26:25 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"


void	LaunchServer(int Port, int BufferSize, int MaxClients) {
	try 
	{
		Server	MainServer(Port, BufferSize , MaxClients);
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
	if (ac == 4)
	{
		LaunchServer(atoi(av[3]), atoi(av[1]), atoi(av[2]));
	}
}