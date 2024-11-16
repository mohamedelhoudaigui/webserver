/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/16 05:19:45 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Config.hpp"
#include "../Include/CGI.hpp"



int main(int ac, char **av ,char **env)
{
	// testing :
	//RunTests();
	TestCGI();

	// try {
	// 	Config c("./Tests/Config/Files/valid");
	// 	c.Parse();

	// 	CGI cgi();
	// 	SocketLayer layer(c);
	// 	layer.OpenServerSockets();
	// 	layer.RunKqueue();
	// }
	// catch (const std::exception& e)
	// {
	// 	std::cout << e.what() << std::endl;	
	// }

}