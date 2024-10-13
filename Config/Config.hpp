/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 02:49:00 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 02:59:20 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIH_HPP


#include <string>
#include <iostream>
#include <vector>
#include <map>


typedef struct RouteConf
{
	std::string					Location;
	std::vector<std::string>	Methods;

}	RouteConf;

typedef struct ServerConf
{
	std::string		Label;
	unsigned int	Port;
	std::string		Root;
	unsigned int	MaxClients;
	std::vector<RouteConf>	Routes;
	
}	ServerConf;

typedef struct ConfigFile
{
	std::vector<ServerConf>					servers;
	std::string								MaxClientBody;
	std::map<unsigned int, std::string>		ErrorPages;
}	ConfigFile;

class Config
{
	public:

	private:		
};

#endif