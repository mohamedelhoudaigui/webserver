/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 04:29:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/14 10:48:20 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Config.hpp"


Config::Config(std::string FileName)
{
	const char*	zbi = FileName.c_str();
	File.open(zbi);
	if (!File.is_open())
		throw std::runtime_error("Error opening config file");
	Result.MaxClientBody = 16000;
	Result.MaxClients = 600;
}

void	Config::Init()
{
	Keys["ClientMaxBodySize"] = 1;
	Keys["ErrorPage"] = 1;
	Keys["ServerName"] = 1;
	Keys["Listen"] = 1;
	Keys["Root"] = 1;
	Keys["Location"] = 1;
	Keys["Methods"] = 1;
	Keys["Index"] = 1;
	Keys["Server"] = 1;
	Keys["Redir"] = 1;
	Keys["AutoIndex"] = 1;
	Keys["MaxClients"] = 1;
	Keys["DefaultMethods"] = 1;
	Keys["AutoIndex"] = 1;
}

void	Config::Parse() // main parser
{
	std::string	Buffer;

	while (std::getline(File, Buffer))  // tokenise and fills this->ConfLines struct
	{
		Buffer = TrimAll(Buffer);
		if (Buffer != "" && Buffer[0] != '#')
		{
			Tokenise(TrimAll(Buffer));
		}
	}
	File.close();
	std::string	Scope = "Global";
	for (int i = 0; i < ConfLines.TokenLines.size(); ++i)
	{
		AssignTokens(ConfLines.TokenLines[i], Scope);
	}
	CheckResult();
}

void	Config::CheckResult()
{
	CheckGlobalParams();
	CheckServers();
}

void	Config::Tokenise(const std::string& LineStr) // tokenizer
{
	TokenLine			Line;
	std::string			Buffer;
	std::stringstream	s(LineStr);
	bool				Key = true;
	
	while (std::getline(s, Buffer, ' '))
	{
		Buffer = TrimAll(Buffer);
		if (Buffer != "")
		{
			Token	token;

			if (Buffer == "}") // close Token
			{
				token.Token = Buffer;
				token.Type = CLOSE;
			}
			else if (Buffer == "{") // open Token
			{
				token.Token = Buffer;
				token.Type = OPEN;
			}

			else if (Key && Buffer != "}" && Buffer != "{") // key Token
			{
				if (Keys[Buffer] != 1)
				{
					throw std::runtime_error("Invalid key in config file :" + Buffer);
				}
				token.Token = Buffer;
				token.Type = KEY;
				Key = false;
			}

			else // value Token
			{
				token.Token = Buffer;
				token.Type = VALUE;
			}

			Line.Tokens.push_back(token);
		}
	}

	ConfLines.TokenLines.push_back(Line);
}

// assign global params from config file
void	Config::AssignGlobalParams(Token& Key, std::vector<Token>& Tokens, std::string& Scope)
{
	if (Key.Token == "Server")
	{
		Scope = "Server";
		return ;
	}
	else if (Key.Token == "ClientMaxBodySize")
		this->Result.MaxClientBody = PairValueNum(Tokens, "ClientMaxBodySize");
	else if (Key.Token == "MaxClients")
		this->Result.MaxClients = PairValueNum(Tokens, "MaxClients");
	else if (Key.Token == "ErrorPage")
		this->Result.ErrorPage = PairValueStr(Tokens, "ErrorPage");
}

// assign server scope from config file
void	Config::AssignServer(Token& Key, std::vector<Token>& Tokens, std::string& Scope)
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


	if (Key.Token == "Location")
	{
		Scope = "Location";
		return ;
	} 

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
void	Config::AssignLocation(Token& Key, std::vector<Token>& Tokens, std::string& Scope)
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

void	Config::AssignTokens(TokenLine& LineTokens, std::string& Scope)
{
	std::vector<Token>	Tokens = LineTokens.Tokens;
	size_t				Ntokens = Tokens.size();

	Token Key = Tokens[0];

	ManageScope(Key.Token, Scope);
	CheckNested(Key.Token, Scope);
	if (Key.Token != "}")
		AssignScope(Key, Tokens, Scope);

}

void	Config::CheckNested(std::string& Key, std::string& Scope)
{
	if (Key == Scope)
	{
		throw std::runtime_error("Error nesting is forbidden");
	}
}

void	Config::ManageScope(std::string& Key, std::string& Scope)
{
	if (Key == "}")
	{ 
		if (Scope == "Global")
			std::runtime_error("Invalid closure tag");
		else if (Scope == "Server")
			Scope = "Global";
		else if (Scope == "Location")
			Scope = "Server";
	}
}

void	Config::AssignScope(Token& Key, std::vector<Token>& Tokens, std::string& Scope)
{
	if (Scope == "Global")
		AssignGlobalParams(Key, Tokens, Scope);
	if (Scope == "Server")
		AssignServer(Key, Tokens, Scope);
	if (Scope == "Location")
		AssignLocation(Key, Tokens, Scope);
}

//assign default values if none are provided except default error page
void	Config::CheckGlobalParams()
{
	// check error page existense
	std::fstream	f;
	f.open(Result.ErrorPage.c_str());
	if (!f.good())
		throw std::runtime_error(Result.ErrorPage + ": Default error page doesn't exist");
	f.close();
}

//check server data
void	Config::CheckServers()
{
	std::vector<ServerConf>	Servers = Result.servers;
	for (std::vector<ServerConf>::iterator it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->Port == 0 || it->Root.empty() || it->ServerName.empty())
			std::runtime_error("Server params error: invalid parameters");
		CheckLocations(it->Routes, *it);
	}
}

void	Config::CheckLocations(std::vector<RouteConf>& Locations, ServerConf& Server)
{
	for (std::vector<RouteConf>::iterator it = Locations.begin(); it != Locations.end(); ++it)
	{
		if (it->Index.empty())
		{
			if (it->AutoIndex == true)
				it->Index = "index.html";
			else
				throw std::runtime_error("Location params error: no index specified");
		}
		else
		{
			std::string	FullPath = "./www/" + Server.Root + "/" + it->Index;
			std::fstream	f;
			f.open(FullPath);
			if (!f.good())
				throw std::runtime_error("Location params error: index file not found: " + FullPath);
		}
		for (std::vector<std::string>::iterator mt = it->Methods.begin(); mt != it->Methods.end(); ++mt)
		{
			if (*mt != "GET" && *mt != "POST" && *mt != "DELETE")
				throw std::runtime_error("Location params error: invalid HTPP method: " + *mt);
		}
		// how to check redirection ??
	}
}



//-- overload :

std::ostream&	operator<<(std::ostream& o, ConfigLines& c)
{
	for (int i = 0; i < c.TokenLines.size(); ++i)
	{
		std::cout << "Line " << i << " :" << std::endl;
		for (int j = 0; j < c.TokenLines[i].Tokens.size(); ++j)
		{
			std::cout << "	" <<
			c.TokenLines[i].Tokens[j].Type << " " <<
			c.TokenLines[i].Tokens[j].Token << std::endl;
		}
	}
	return o;
}

std::ostream&	operator<<(std::ostream& o, RouteConf& r)
{
	o << "		Index : " << r.Index << std::endl;
	o << "		Redir : " << r.Redir << std::endl;
	o << "		AutoIndex : " << r.AutoIndex << std::endl;
	o << "		Loaction : " << r.Location << std::endl;
	o << "		Methods : ";
	for (int i = 0; i < r.Methods.size(); ++i)
	{
		o << r.Methods[i] << " ";
	}
	o << std::endl;
	return o;
}

std::ostream&	operator <<(std::ostream& o, ServerConf& s)
{
	o << "	Port :" << s.Port << std::endl;
	o << "	Root folder : " << s.Root << std::endl;
	o << "	Server name : " << s.ServerName << std::endl;
	for (int i = 0; i < s.Routes.size(); ++i)
	{
		o << "\n=========Route==========" << std::endl;
		o << s.Routes[i];
		o << "========================" << std::endl;
	}
	return o;
}

std::ostream&	operator<<(std::ostream& o, Config& c)
{
	ConfigFile	res = c.GetResult();
	o << "Error page : " << res.ErrorPage << std::endl;
	o << "Max clients : " << res.MaxClients << std::endl;
	o << "Max client body size : " << res.MaxClientBody << std::endl;
	std::cout << "Servers :" << std::endl;
	for (int i = 0; i < res.servers.size(); ++i)
	{
		o << "\n---------Server---------" << std::endl;
		o << res.servers[i];
		o << "-----------------------" << std::endl;
	}
	return o;
}

//-- getters :

ConfigLines	Config::GetLines()
{
	return (this->ConfLines);
}

ConfigFile	Config::GetResult()
{
	return (this->Result);
}