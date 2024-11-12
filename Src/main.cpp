/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/12 00:05:06 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Config.hpp"

//void	LaunchServer(int Port) {
//	try 
//	{
//		Server	MainServer(Port, 16384, 500); // BufferSize - MaxClients
//		MainServer.BindServer();
//		MainServer.GetServerInfo();
//		MainServer.ListenServer();
//		MainServer.SelectSetup();
//	}
//	catch (const std::exception& e)
//	{
//		std::cerr << e.what() << std::endl;
//		exit(1);
//	}
//}


int main(int ac, char **av) {
	// testing :
	//RunTests();

	try {
		Config c("./Tests/Config/Files/valid");
		c.Parse();

		SocketLayer layer(c);
		layer.OpenServerSockets();
		layer.SelectSetup();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;	
	}
}