/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 04:29:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 13:14:38 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Config.hpp"


Config::Config(std::string FileName)
{
	const char*	zbi = FileName.c_str();
	File.open(zbi);
	if (!File.is_open())
		throw std::runtime_error("Error opening config file");
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

void	Config::Parse()
{
	std::string	Buffer;

	while (std::getline(File, Buffer))
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
}



void	Config::Tokenise(const std::string& LineStr)
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

			if (Buffer == "}")
			{
				token.Token = Buffer;
				token.Type = CLOSE;
			}
			else if (Buffer == "{")
			{
				token.Token = Buffer;
				token.Type = OPEN;
			}

			else if (Key && Buffer != "}" && Buffer != "{") // key
			{
				if (Keys[Buffer] != 1)
				{
					throw std::runtime_error("Invalid key in config file :" + Buffer);
				}
				token.Token = Buffer;
				token.Type = KEY;
				Key = false;
			}

			else
			{
				token.Token = Buffer;
				token.Type = VALUE;
			}

			Line.Tokens.push_back(token);
		}
	}

	ConfLines.TokenLines.push_back(Line);
}

void	Config::AssignGlobalParams(Token& Key, std::vector<Token>& Tokens)
{
	if (Key.Token == "ClientMaxBodySize")
		this->Result.MaxClientBody = PairValueNum(Tokens, "ClientMaxBodySize");
	if (Key.Token == "MaxClients")
		this->Result.MaxClients = PairValueNum(Tokens, "MaxClients");
	if (Key.Token == "ErrorPage")
	{
		std::string Path = PairValueStr(Tokens, "ErrorPage");
		std::ifstream f(Path.c_str());
		if (f.good() == false)
			throw std::runtime_error(Tokens[1].Token + " ErrorFile doesnt exist");
		f.close();
		this->Result.ErrorPage = Path;
	}
}

void	Config::AssignServer(Token& Key, std::vector<Token>& Tokens, std::string& Scope)
{
	if (Key.Token == "Server")
	{
		if (Tokens.size() != 2 || Tokens[1].Token != "{")
			throw std::runtime_error("Server parsing error");
		ServerConf	Server;
		this->Result.servers.push_back(Server);
	}
	if (Key.Token == "Listen")
	{
		if (Result.servers.size() == 0)
			throw std::runtime_error("Error in listen attribute");
		this->Result.servers.back().Port = PairValueNum(Tokens, "Listen");
	}

	if (Key.Token ==  "ServerName")
	{
		if (Result.servers.size() == 0)
			throw std::runtime_error("Error in listen attribute");
		this->Result.servers.back().ServerName = PairValueStr(Tokens, "ServerName");
	}

	if (Key.Token == "Root")
	{
		if (Result.servers.size() == 0)
			throw std::runtime_error("Error in listen attribute");
		this->Result.servers.back().Root = PairValueStr(Tokens, "ServerName");
	}

	if (Key.Token == "}")
	{
		Scope == "Global";
	}
}

void	Config::AssignTokens(TokenLine& LineTokens, std::string& Scope)
{
	std::vector<Token>	Tokens = LineTokens.Tokens;
	size_t				Ntokens = Tokens.size();

	Token Key = Tokens[0];

	if (Key.Token == "Server")
	{
		Scope = "Server";
	}
	else if (Key.Token == "Location")
	{
		Scope = "Location";
	}
	if (Scope == "Global")
	{
		AssignGlobalParams(Key, Tokens);
	}
	else if (Scope == "Server")
	{
		AssignServer(Key, Tokens, Scope);
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
			std::cout << "	" << c.TokenLines[i].Tokens[j].Type << " " << c.TokenLines[i].Tokens[j].Token << std::endl;
		}
	}
	return o;
}

std::ostream&	operator<<(std::ostream& o, RouteConf& r)
{
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
		o << "========================" << std::endl;
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
		o << "-----------------------" << std::endl;
		o << res.servers[i];
		o << "-----------------------" << std::endl;
	}
	return o;
}

ConfigLines	Config::GetLines()
{
	return (this->ConfLines);
}

ConfigFile	Config::GetResult()
{
	return (this->Result);
}