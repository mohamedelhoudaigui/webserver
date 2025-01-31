/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Assigner.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:32:18 by mel-houd          #+#    #+#             */
/*   Updated: 2025/01/26 15:11:35 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

void	Config::AssignTokens()
{
    std::vector<TokenLine>::iterator it;
    for (it = ConfLines.TokenLines.begin();  it != ConfLines.TokenLines.end(); ++it)
    {
        std::vector<Token>&	Tokens = it->Tokens;
        size_t				Ntokens = Tokens.size();

        Token& Key = Tokens[0];
        AssignGlobalParams(Key, Tokens);
        AssignServer(Key, Tokens);
        AssignLocation(Key, Tokens);
    }
    Logger(DEBUG, "tokens assigning passed");
}

// assign global params from config file
void	Config::AssignGlobalParams(Token& Key, std::vector<Token>& Tokens)
{
	if (Key.Token == "DefaultErrorPage")
		this->Result.Default.DefaultErrorPage = PairValueStr(Tokens, "DefaultErrorPage");
	else if (Key.Token == "ClientBodyLimit")
		this->Result.Default.MaxBodyLimit = PairValueNum(Tokens, "ClientBodyLimit", 100000000);
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
		this->Result.servers.back().Port = PairValueNum(Tokens, "Listen", MAX_PORT);
	else if (Key.Token ==  "ServerName")
		this->Result.servers.back().ServerName = PairValueStr(Tokens, "ServerName");
	else if (Key.Token == "Host")
		this->Result.servers.back().Host = PairValueStr(Tokens, "Host");
	else if (Key.Token == "Root")
		this->Result.servers.back().Root = PairValueStr(Tokens, "Root");
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
		Location.Root = this->Result.servers.back().Root;
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
	else if (Key.Token == "IsCgi")
		this->Result.servers.back().Routes.back().Root =  PairValueBool(Tokens, "IsCgi");
}


void	Config::CheckResult()
{
	CheckServers();
}

//check server data
void	Config::CheckServers()
{
	std::vector<ServerConf>	Servers = Result.servers;
	std::vector<ServerConf>::iterator	it;
	std::vector<std::string>			ServerNames;

	for (it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->ServerName.empty())
			throw std::runtime_error("Server without ServerName attribute");
		ServerNames.push_back(it->ServerName);
	}

	sort(ServerNames.begin(), ServerNames.end());

	for (int i = 1; i < ServerNames.size(); ++i)
	{
		if (ServerNames[i - 1] == ServerNames[i])
			throw std::runtime_error("Duplicated ServerName attribute");
	}

	for (it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->Host.empty())
			throw std::runtime_error("Server params error invalid host");
        if (it->Root.empty())
			throw std::runtime_error("Server params error invalid root");

		CheckLocations(it->Routes, *it);
	}
}

void	Config::CheckLocations(std::vector<RouteConf>& Locations, ServerConf& Server)
{
	for (std::vector<RouteConf>::iterator it = Locations.begin(); it != Locations.end(); ++it)
	{
		CheckMethods(*it);
		CheckAutoIndex(*it);
	}
}

void	Config::CheckAutoIndex(RouteConf& Location)
{
	if (Location.Index.empty())
	{
		if (Location.AutoIndex == true)
			Location.Index = "index.html";
		else
			throw std::runtime_error("Location params error: no index specified");
	}
	else
		std::string	FullPath = Location.Root + "/" + Location.Index;
}


void	Config::CheckMethods(RouteConf& Loaction)
{
	std::vector<std::string>::iterator it;
	for (it = Loaction.Methods.begin(); it !=Loaction.Methods.end(); ++it)
	{
		if (*it != "GET" && *it != "POST" && *it != "DELETE" && *it != "PUT")
			throw std::runtime_error("Location params error invalid HTPP method: " + *it);
	}
}
