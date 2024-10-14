/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 08:34:31 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 14:08:09 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>

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
	bool						AutoIndex;
	std::string					Redir;
	std::string					Index;
	std::vector<std::string>	Methods;

}	RouteConf;

typedef struct ServerConf
{
	unsigned int	Port;
	std::string		Root;
	std::string		ServerName;
	std::vector<RouteConf>	Routes;
	
}	ServerConf;

typedef struct ConfigFile
{
	unsigned int							MaxClientBody;
	unsigned int							MaxClients;
	std::string								ErrorPage;
	std::vector<ServerConf>					servers;
}	ConfigFile;


#endif