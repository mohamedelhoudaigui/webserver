/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 08:46:49 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Config.hpp"

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
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}


int main(int ac, char **av) {
	//if (ac == 2)
	//{
	//	LaunchServer(atoi(av[1]));
	//}
	if (ac != 2)
		return 1;
	try{
		Config	g(av[1]);
		g.Init();
		g.Parse();
		std::cout << g.GetResult().ErrorPage << std::endl;
		std::cout << g.GetResult().MaxClientBody << std::endl;
		std::cout << g.GetResult().MaxClients << std::endl;
		//ConfigLines	Lines = g.GetLines();
		//std::cout << Lines;
	} catch (const std::exception& e){
		std::cout << e.what() << std::endl;
		exit(2);
	}
}