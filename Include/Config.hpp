/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 02:49:00 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 05:32:41 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <unistd.h>

#include "Helper.hpp"

enum Tokens
{
	KEY,
	VALUE,
	COMMENT,
	ENDLINE,
	OPEN,
	CLOSE,
};


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
	unsigned long							MaxClientBody;
	unsigned int							MaxClients;
	std::string								ErrorPage;
	std::vector<ServerConf>					servers;
}	ConfigFile;

class Config
{
	public:
		Config(std::string FileName);
		void	Init();
		void	Parse();

	private:
		std::map<std::string, int>	Keys;
		ConfigFile					Result;
		std::fstream				File;
};


#endif