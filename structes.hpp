/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 04:38:30 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/20 08:36:11 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTES_HPP
#define STRUCTES_HPP

#include <string>

typedef	struct
{
	int									fd;
	std::string							host;
	int									port;
	std::map<std::string, std::string>	routes;
	std::vector<std::string>			methods;
}		server_config;


#endif