/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:22:32 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/12 02:08:07 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock.hpp"

int main()
{

	// change ip address
	int	ports[2] = {4545, 4546};
    Sock server(ports, "10.13.11.5");
	server.init_server();
	server.close_sock();
    return 0;
}