/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 02:49:00 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 06:17:12 by mel-houd         ###   ########.fr       */
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

enum TokenType
{
	KEY = 0,
	VALUE = 1,
	OPEN = 2,
	CLOSE = 3,
};

typedef struct Token
{
	std::string	Token;
	TokenType	Type;

}	Token;

typedef struct TokenLine
{
	std::vector<Token>	Tokens;
}	TokenLine;

typedef struct ConfigLines
{
	std::vector<TokenLine>	TokenLines;
}			ConfigLines;

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
		void		Init();
		void		Parse();
		void		Tokenise(std::string LineStr);
		ConfigLines	GetLines();

	private:
		std::map<std::string, int>	Keys;
		ConfigFile					Result;
		std::fstream				File;
		ConfigLines					ConfLines;
};

std::ostream&	operator<<(std::ostream& o, ConfigLines& c);


#endif