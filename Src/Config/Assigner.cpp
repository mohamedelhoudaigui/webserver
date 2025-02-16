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
	(void)Key;
	(void)Tokens;
}

// assign server scope from config file
void	Config::AssignServer(Token& Key, std::vector<Token>& Tokens)
{
	if (Key.Token == "Server")
	{
		this->Result.SetEmptyServer();
		return ;
	}
	ServerConf& last_server = this->Result.GetLastServer();

	if (Key.Token == "Listen")
		last_server.SetPort(PairValueNum(Tokens, "Listen", MAX_PORT));
	else if (Key.Token ==  "ServerName")
		last_server.SetServerName(PairValueStr(Tokens, "ServerName"));
	else if (Key.Token == "Host")
		last_server.SetHost(PairValueStr(Tokens, "Host"));
	else if (Key.Token == "Root")
		last_server.SetRoot(PairValueStr(Tokens, "Root"));
}

// assign server scope from config file
void	Config::AssignLocation(Token& Key, std::vector<Token>& Tokens)
{
	(void)Key;
	(void)Tokens;
	// if (Key.Token == "Location")
	// {
	// 	RouteConf	Location;
	// 	Location.Location = Tokens[1].Token;
	// 	Location.AutoIndex = false;
	// 	Location.DirList = false;
	// 	Location.Default = &Result.Default;
	// 	Location.IsCgi = false;
	// 	Location.Root = this->Result.servers.back().Root;
	// 	this->Result.servers.back().Routes.push_back(Location);
	// 	return ;
	// }
	
	// if (Key.Token == "Index")
	// 	this->Result.servers.back().Routes.back().Index = PairValueStr(Tokens, "Index");
	// else if (Key.Token == "Redir")
	// 	this->Result.servers.back().Routes.back().Redir = PairValueStr(Tokens, "Redir");
	// else if (Key.Token == "UploadDir")
	// 	this->Result.servers.back().Routes.back().UploadDir =  PairValueStr(Tokens, "UploadDir");
	// else if (Key.Token == "AutoIndex")
	// 	this->Result.servers.back().Routes.back().AutoIndex =  PairValueBool(Tokens, "AutoIndex");
	// else if (Key.Token == "DirList")
	// 	this->Result.servers.back().Routes.back().DirList =  PairValueBool(Tokens, "DirList");
	// else if (Key.Token == "Methods")
	// 	this->Result.servers.back().Routes.back().Methods =  MultiValueStr(Tokens, "Methods");
	// else if (Key.Token == "IsCgi")
	// 	this->Result.servers.back().Routes.back().Root =  PairValueBool(Tokens, "IsCgi");
    // else if (Key.Token == "CgiName")
	// 	this->Result.servers.back().Routes.back().CgiName =  PairValueStr(Tokens, "CgiName");

}


void	Config::CheckResult()
{
	CheckServers();
}

//check server data
void	Config::CheckServers()
{
	// std::vector<ServerConf>	Servers = Result.servers;
	// std::vector<ServerConf>::iterator	it;
	// std::vector<std::string>			ServerNames;

	// for (it = Servers.begin(); it != Servers.end(); ++it)
	// {
	// 	if (it->ServerName.empty())
	// 		throw std::runtime_error("Server without ServerName attribute");
	// 	ServerNames.push_back(it->ServerName);
	// }

	// sort(ServerNames.begin(), ServerNames.end());

	// for (size_t i = 1; i < ServerNames.size(); ++i)
	// {
	// 	if (ServerNames[i - 1] == ServerNames[i])
	// 		throw std::runtime_error("Duplicated ServerName attribute");
	// }

	// for (it = Servers.begin(); it != Servers.end(); ++it)
	// {
	// 	if (it->Host.empty())
	// 		throw std::runtime_error("Server params error invalid host");
    //     if (it->Root.empty())
	// 		throw std::runtime_error("Server params error invalid root");

	// 	CheckLocations(it->Routes);
	// }
}

void	Config::CheckLocations(std::vector<RouteConf>& Locations)
{
	(void)Locations;
	// for (std::vector<RouteConf>::iterator it = Locations.begin(); it != Locations.end(); ++it)
	// {
	// 	CheckMethods(*it);
	// 	CheckAutoIndex(*it);
	// }
}

void	Config::CheckAutoIndex(RouteConf& Location)
{
	(void)Location;
	// if (Location.Index.empty())
	// {
	// 	if (Location.AutoIndex == true)
	// 		Location.Index = "index.html";
	// 	else
	// 		throw std::runtime_error("Location params error: no index specified");
	// }
	// else
	// 	std::string	FullPath = Location.Root + "/" + Location.Index;
}


void	Config::CheckMethods(RouteConf& Location)
{
	(void)Location;
	// std::vector<std::string>::iterator it;
	// for (it = Loaction.Methods.begin(); it !=Loaction.Methods.end(); ++it)
	// {
	// 	if (*it != "GET" && *it != "POST" && *it != "DELETE" && *it != "PUT")
	// 		throw std::runtime_error("Location params error invalid HTPP method: " + *it);
	// }
}

void    Config::CheckCgiData(RouteConf& Location)
{
	(void)Location;
    // if (Location.IsCgi == true)
    // {
    //     if (Location.CgiName.empty())
    //         throw std::runtime_error("cgi is on, put no CgiName is provided");
    // }
}
