/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 08:34:31 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/24 06:47:38 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>
#include <map>
#include <algorithm>

enum Error
{
	NotFound = 400,
};

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

typedef struct DefaultConf
{
	unsigned int				DefaultMaxClients;
	unsigned int				DefaultMaxClientBody;
	std::string					DefaultErrorPage;
	std::string					DefaultRoot;
	std::string					DefaultUploadDir;
	std::string					DefaultIndex;

	unsigned int				GetDefaultMaxClients();
	unsigned int				GetDefaultMaxBody();
	std::string&				GetDefaultErrorPage();
	std::string&				GetDefaultRoot();
	std::string&				GetDefaultUploadDir();
	std::string&				GetDefaultIndex();

}			DefaultConf;

typedef struct RouteConf
{
	DefaultConf*					Default;

	bool							DirList;
	bool							AutoIndex;
	std::string						Location;
	std::string						Redir;
	std::string						Index;
	std::string						UploadDir;
	std::string						Root;
	unsigned int					MaxClientBody;
	std::vector<std::string>		Methods;

	std::string&					GetLocationPath();
	std::string&					GetIndex();
	std::string&					GetRoot();
	unsigned int					GetMaxBody();
	bool							CheckDirectoryListing();
	bool							CheckAutoIndex();
	bool							CheckRedirection();
	bool							CheckUploadDir();
	bool							CheckMethod(std::string& Method);
	std::string&					GetRedirection(); // throws
	std::string&					GetUploadDir();

}	RouteConf;



typedef struct ServerConf
{
	DefaultConf*						Default;

	unsigned int						MaxClients;
	std::string							Host;
	std::string							ServerName;
	std::vector<unsigned int>			Port;
	std::vector<RouteConf>				Routes;
	std::map<unsigned int, std::string>	ErrorPage;

	std::string&						GetHost();
	std::string&						GetServerName();
	unsigned int						GetMaxClients();
	bool								CheckPort(unsigned int Port);
	bool								CheckLocation(std::string& LocationPath);
	std::string&						GetErrorPage(unsigned int ErrorCode); // throws
	RouteConf& 							GetLocation(std::string& LocationPath); // throws
	
}	ServerConf;



typedef struct ConfigFile
{
	std::vector<ServerConf>	servers;
	DefaultConf				Default;
	bool					CheckServer(std::string& Host, unsigned int Port);
	ServerConf&				GetServer(std::string& Host, unsigned int Port); // throws

}	ConfigFile;


#endif