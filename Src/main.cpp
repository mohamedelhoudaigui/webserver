/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:43:46 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/14 03:49:34 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"
#include "../Include/Config.hpp"



int main(int ac, char **av) {
	// testing :
	//RunTests();

	try {
		Config c("./Tests/Config/Files/valid");
		c.Parse();

		SocketLayer layer(c);
		layer.OpenServerSockets();
		layer.RunKqueue();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;	
	}
}