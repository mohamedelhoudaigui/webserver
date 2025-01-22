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

void	Config::CheckResult()
{
	CheckGlobalParams();
	CheckServers();
	CheckServerNames();
}

//assign default values if none are provided
void	Config::CheckGlobalParams()
{
	// check files existense
	CheckFile(Result.Default.DefaultErrorPage, "DefaultErrorPage");
	CheckFile(Result.Default.DefaultIndex, "DefaultIndex");
	CheckFolder(Result.Default.DefaultRoot, "DefaultRoot");
	CheckFolder(Result.Default.DefaultUploadDir, "DefaultUploadDir");
	if (Result.Default.DefaultMaxClientBody == 0)
	{
		throw std::runtime_error("missing or invalid default configuration (DefaultMaxClients or DefaultMaxClientBody)");
	}
}

//Check uniques ServerNames

void	Config::CheckServerNames()
{
	std::vector<ServerConf>::iterator	it;
	std::vector<std::string>			ServerNames;

	for (it = Result.servers.begin(); it != Result.servers.end(); ++it)
		ServerNames.push_back(it->ServerName);

	sort(ServerNames.begin(), ServerNames.end());

	for (int i = 1; i < ServerNames.size(); ++i)
	{
		if (ServerNames[i - 1] == ServerNames[i])
			throw std::runtime_error("Duplicated ServerName attribute");
	}
}


//check server data
void	Config::CheckServers()
{
	std::vector<ServerConf>	Servers = Result.servers;
	for (std::vector<ServerConf>::iterator it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->Host.empty())
			throw std::runtime_error("Server params error: invalid parameters");

		CheckPorts(it->Port);
		CheckLocations(it->Routes, *it);
	}
}

void	Config::CheckLocations(std::vector<RouteConf>& Locations, ServerConf& Server)
{
	for (std::vector<RouteConf>::iterator it = Locations.begin(); it != Locations.end(); ++it)
	{
		CheckMethods(*it);
	}
}

void	Config::CheckPorts(std::vector<unsigned int>& Ports)
{
	std::vector<unsigned int>::iterator it;

	if (Ports.size() == 0)
		throw std::runtime_error("No port assigned to server");
	for (it = Ports.begin(); it != Ports.end(); ++it)
	{
		if (*it > 65535)
			throw std::runtime_error("Invalid port number");
	}
}

void	Config::CheckIndex(RouteConf& Location)
{
	if (Location.Index.empty())
	{
		if (Location.AutoIndex == true)
			Location.Index = "index.html";
		else
			throw std::runtime_error("Location params error: no index specified");
	}
	else
	{
		std::string	FullPath = Location.Root + "/" + Location.Index;
		CheckFile(FullPath, "Index");
	}
}


void	Config::CheckMethods(RouteConf& Loaction)
{
	std::vector<std::string>::iterator it;
	for (it = Loaction.Methods.begin(); it !=Loaction.Methods.end(); ++it)
	{
		if (*it != "GET" && *it != "POST" && *it != "DELETE" && *it != "PUT")
			throw std::runtime_error("Location params error: invalid HTPP method: " + *it);
	}
}
