/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Assigner.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:32:18 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/15 23:13:43 by mel-houd         ###   ########.fr       */
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
		ServerConf	Server;
		Server.Port = 0;
		this->Result.servers.push_back(Server);
		return ;
	}
	else if (Key.Token == "Listen")
		this->Result.servers.back().Port = PairValueNum(Tokens, "Listen");
	else if (Key.Token ==  "ServerName")
		this->Result.servers.back().ServerName = PairValueStr(Tokens, "ServerName");
	else if (Key.Token == "Root")
		this->Result.servers.back().Root = PairValueStr(Tokens, "Root");
	else if (Key.Token == "Host")
		this->Result.servers.back().Host = PairValueStr(Tokens, "Host");
}

// assign server scope from config file
void	Config::AssignLocation(Token& Key, std::vector<Token>& Tokens)
{
	if (Key.Token == "Location")
	{
		RouteConf	Location;
		Location.Location = Tokens[1].Token;
		Location.AutoIndex = false;
		Location.DirList = false;
		this->Result.servers.back().Routes.push_back(Location);
		return ;
	}
	if (Key.Token == "Index")
		this->Result.servers.back().Routes.back().Index = PairValueStr(Tokens, "Index");
	else if (Key.Token == "Redir")
		this->Result.servers.back().Routes.back().Redir = PairValueStr(Tokens, "Redir");
	else if (Key.Token == "UploadDir")
		this->Result.servers.back().Routes.back().UpDir =  PairValueStr(Tokens, "UpDir");
	else if (Key.Token == "AutoIndex")
		this->Result.servers.back().Routes.back().AutoIndex =  PairValueBool(Tokens, "AutoIndex");
	else if (Key.Token == "DirList")
		this->Result.servers.back().Routes.back().DirList =  PairValueBool(Tokens, "DirList");
	else if (Key.Token == "Methods")
		this->Result.servers.back().Routes.back().Methods =  MultiValueStr(Tokens, "Methods");
}

void	Config::AssignTokens()
{
	for (std::vector<TokenLine>::iterator it = ConfLines.TokenLines.begin();  it != ConfLines.TokenLines.end(); ++it)
	{
		std::vector<Token>	Tokens = it->Tokens;
		size_t				Ntokens = Tokens.size();

		Token Key = Tokens[0];
		AssignGlobalParams(Key, Tokens);
		AssignServer(Key, Tokens);
		AssignLocation(Key, Tokens);
	}
}