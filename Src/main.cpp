/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/14 11:36:07 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Config.hpp"
#include "../Include/Response.hpp"

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
	// testing :
	// RunTests();
	std::map<std::string, std::string>	headers;
		headers.insert(std::make_pair("content-length", "15344"));
		headers.insert(std::make_pair("content-type", "font/woff"));
		headers.insert(std::make_pair("server", "nginx"));
		headers.insert(std::make_pair("date", "tue, October 15, 2024 12:44AM GMT"));
	HttpResponse rep(headers, "HTTP/1.1", "OK", " ", 200);
	std::cout << rep;
}