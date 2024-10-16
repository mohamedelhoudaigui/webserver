/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 08:34:31 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/16 03:45:30 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>
#include <map>

enum Scope
{
	GLOBAL = 0,
	SERVER = 1,
	LOCATION = 2,
};

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
	bool						DirList;
	bool						AutoIndex;
	std::string					Location;
	std::string					Redir;
	std::string					Index;
	std::string					UpDir;
	std::string					Root;
	unsigned int				MaxClientBody;
	std::vector<std::string>	Methods;

}	RouteConf;



typedef struct ServerConf
{
	unsigned int					MaxClients;
	std::string							Host;
	std::vector<unsigned int>			Port;
	std::string							ServerName;
	std::map<unsigned int, std::string>	ErrorPage;
	std::vector<RouteConf>				Routes;
	
}	ServerConf;



typedef struct ConfigFile
{
	std::string								DefaultErrorPage;
	std::vector<ServerConf>					servers;
}	ConfigFile;


#endif