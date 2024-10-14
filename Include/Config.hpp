/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 02:49:00 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/14 18:50:14 by mel-houd         ###   ########.fr       */
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
#include <unistd.h>

#include "Helper.hpp"
#include "Types.hpp"


class Config
{
	public:
		//Config.cpp
		Config(std::string FileName);
		void		Init();
		void		Parse();
		ConfigLines	GetLines();
		ConfigFile	GetResult();
	
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
		void		CheckGlobalParams();
		void		CheckServers();
		void		CheckLocations(std::vector<RouteConf>& Locations, ServerConf& Server);

		//Syntax.cpp
		void		CheckSyntaxError();

	private:
		std::map<std::string, int>	Keys;
		ConfigFile					Result;
		std::fstream				File;
		ConfigLines					ConfLines;
};

void	RunTests();

std::ostream&	operator<<(std::ostream& o, ConfigLines& c);
std::ostream&	operator<<(std::ostream& o, Config& c);
std::ostream&	operator <<(std::ostream& o, ServerConf& s);
std::ostream&	operator<<(std::ostream& o, RouteConf& r);


#endif