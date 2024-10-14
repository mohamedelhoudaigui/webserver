/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Assigner.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:32:18 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/14 18:40:12 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

// assign global params from config file
void	Config::AssignGlobalParams(Token& Key, std::vector<Token>& Tokens)
{
	if (Key.Token == "ClientMaxBodySize")
		this->Result.MaxClientBody = PairValueNum(Tokens, "ClientMaxBodySize");
	else if (Key.Token == "MaxClients")
		this->Result.MaxClients = PairValueNum(Tokens, "MaxClients");
	else if (Key.Token == "ErrorPage")
		this->Result.ErrorPage = PairValueStr(Tokens, "ErrorPage");
}

// assign server scope from config file
void	Config::AssignServer(Token& Key, std::vector<Token>& Tokens)
{
	if (Key.Token == "Server")
	{
		if (Tokens.size() != 2 || Tokens[1].Token != "{")
			throw std::runtime_error("Server scope parsing error");
		ServerConf	Server;
		Server.Port = 0;
		this->Result.servers.push_back(Server);
		return ;
	}

	if (Result.servers.size() == 0)
		throw std::runtime_error("Attribute of Server but there is none");

	else if (Key.Token == "Listen")
	{
		this->Result.servers.back().Port = PairValueNum(Tokens, "Listen");
	}


	else if (Key.Token ==  "ServerName")
	{
		this->Result.servers.back().ServerName = PairValueStr(Tokens, "ServerName");
	}

	else if (Key.Token == "Root")
	{
		this->Result.servers.back().Root = PairValueStr(Tokens, "Root");
	}
	
	else
		throw std::runtime_error(Key.Token + ": Invalid keyword in server scope");
}

// assign server scope from config file
void	Config::AssignLocation(Token& Key, std::vector<Token>& Tokens)
{
	if (Key.Token == "Location")
	{
		if (Tokens.size() != 3 || Tokens[2].Token != "{")
			throw std::runtime_error("Location scope parsing error");
		RouteConf	Location;
		Location.Location = Tokens[1].Token;
		Location.AutoIndex = false;
		this->Result.servers.back().Routes.push_back(Location);
		return ;
	}

	if (Result.servers.back().Routes.size() == 0)
			throw std::runtime_error("Attribute of Location but there is none");

	if (Key.Token == "Index")
	{
		this->Result.servers.back().Routes.back().Index = PairValueStr(Tokens, "Index");
	}

	else if (Key.Token == "Redir")
	{
		this->Result.servers.back().Routes.back().Redir = PairValueStr(Tokens, "Redir");
	}

	else if (Key.Token == "Methods")
	{
		this->Result.servers.back().Routes.back().Methods =  MultiValueStr(Tokens, "Methods");
	}

	else if (Key.Token == "AutoIndex")
	{
		this->Result.servers.back().Routes.back().AutoIndex =  PairValueBool(Tokens, "AutoIndex");
	}
	else
		throw std::runtime_error(Key.Token + "Invalid keyword in location scope");
}