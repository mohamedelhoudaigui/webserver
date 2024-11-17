/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Assigner.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:32:18 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/17 15:49:12 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

// assign global params from config file
void	Config::AssignGlobalParams(Token& Key, std::vector<Token>& Tokens)
{
	if (Key.Token == "DefaultErrorPage")
		this->Result.Default.DefaultErrorPage = PairValueStr(Tokens, "DefaultErrorPage");
	else if (Key.Token == "DefaultIndex")
		this->Result.Default.DefaultIndex = PairValueStr(Tokens, "DefaultIndex");
	else if (Key.Token == "DefaultUploadDir")
		this->Result.Default.DefaultUploadDir = PairValueStr(Tokens, "DefaultUploadDir");
	else if (Key.Token == "DefaultRoot")
		this->Result.Default.DefaultRoot = PairValueStr(Tokens, "DefaultRoot");
	else if (Key.Token == "DefaultMaxClientBody")
		this->Result.Default.DefaultMaxClientBody = PairValueNum(Tokens, "DefaultMaxClientBody");
}

// assign server scope from config file
void	Config::AssignServer(Token& Key, std::vector<Token>& Tokens)
{
	if (Key.Token == "Server")
	{
		ServerConf	Server;
		Server.Default = &Result.Default;
		this->Result.servers.push_back(Server);
		return ;
	}
	else if (Key.Token == "Listen")
		MultiValueNum(Tokens, "Listen", this->Result.servers.back().Port);
	else if (Key.Token ==  "ServerName")
		this->Result.servers.back().ServerName = PairValueStr(Tokens, "ServerName");
	else if (Key.Token == "Host")
		this->Result.servers.back().Host = PairValueStr(Tokens, "Host");
	else if (Key.Token == "ErrorPage")
		ParseErrorPage(Tokens, this->Result.servers.back().ErrorPage);
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
		Location.Default = &Result.Default;
		Location.IsCgi = false;
		this->Result.servers.back().Routes.push_back(Location);
		return ;
	}
	
	if (Key.Token == "Index")
		this->Result.servers.back().Routes.back().Index = PairValueStr(Tokens, "Index");
	else if (Key.Token == "Redir")
		this->Result.servers.back().Routes.back().Redir = PairValueStr(Tokens, "Redir");
	else if (Key.Token == "UploadDir")
		this->Result.servers.back().Routes.back().UploadDir =  PairValueStr(Tokens, "UploadDir");
	else if (Key.Token == "AutoIndex")
		this->Result.servers.back().Routes.back().AutoIndex =  PairValueBool(Tokens, "AutoIndex");
	else if (Key.Token == "DirList")
		this->Result.servers.back().Routes.back().DirList =  PairValueBool(Tokens, "DirList");
	else if (Key.Token == "Methods")
		this->Result.servers.back().Routes.back().Methods =  MultiValueStr(Tokens, "Methods");
	else if (Key.Token == "Root")
		this->Result.servers.back().Routes.back().Root =  PairValueStr(Tokens, "Root");
	else if (Key.Token == "IsCgi")
		this->Result.servers.back().Routes.back().Root =  PairValueBool(Tokens, "IsCgi");
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