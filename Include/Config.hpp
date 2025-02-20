#pragma once

#include <string>
#include <iostream> 
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <unistd.h>

#include "Tools.hpp"
#include "Types.hpp"
#include "Storage.hpp"


class Config
{
	public:
		Config();
		Config(std::string FileName);
		Config& operator=(const Config& other);
		~Config();

		void		Parse();
		void		Tokeniser();
		void		TokeniseLine(const std::string& LineStr);
		void		AssignTokens();
		void		AssignGlobalParams(Token& Key, std::vector<Token>& Tokens);
		void		AssignServer(Token& Key, std::vector<Token>& Tokens);
		void		AssignLocation(Token& Key, std::vector<Token>& Tokens);

		void		CheckResult();

		void		CheckServers();
		void		CheckHost();
		void		CheckServerName();
		void		CheckRoot();

		void		CheckLocations();
		void		CheckCgi(const RouteConf& Location);
		void		CheckIndex(const RouteConf& Location);
		void		CheckMethods(const RouteConf& Location);

		void		CheckScope(std::vector<Token>& Tokens, Scope& s);
		void		CheckScopeKeyWord(std::string& Key, std::vector<std::string>& Keys);
		void		CheckSyntaxError();

		//Getters.cpp
		ConfigLines		GetLines();
		ConfigFile&		GetResult();
		std::fstream&	GetLog();

	private:
		std::vector<std::string>	GlobalKeys;
		std::vector<std::string>	ServerKeys;
		std::vector<std::string>	LocationKeys;
		ConfigFile					Result;
		std::fstream				File;
		ConfigLines					ConfLines;
};

void	TestConfig();
