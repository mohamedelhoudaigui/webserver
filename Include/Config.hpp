/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 02:49:00 by mel-houd          #+#    #+#             */
/*   Updated: 2025/01/26 11:14:10 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP


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


class Config
{
	public:
		//Config.cpp
		Config(std::string FileName);
		void		Parse();
		//Tokens.cpp
		void		Tokeniser();
		void		TokeniseLine(const std::string& LineStr);
		void		AssignTokens();

		//Assigner.cpp
		void		AssignGlobalParams(Token& Key, std::vector<Token>& Tokens);
		void		AssignServer(Token& Key, std::vector<Token>& Tokens);
		void		AssignLocation(Token& Key, std::vector<Token>& Tokens);

		//Checker.cpp
		void		CheckResult();
		void		CheckServers();
		void		CheckAutoIndex(RouteConf& Location);
		void		CheckMethods(RouteConf& Location);
		void		CheckLocations(std::vector<RouteConf>& Locations, ServerConf& Server);

		//Syntax.cpp
		void		CheckScope(std::vector<Token>& Tokens, Scope& s);
		void		CheckScopeKeyWord(std::string& Key, std::vector<std::string>& Keys);
		void		CheckSyntaxError();

		//Getters.cpp
		ConfigLines		GetLines();
		ConfigFile		GetResult();
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



#endif
