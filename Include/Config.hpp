/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 02:49:00 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 14:04:19 by mel-houd         ###   ########.fr       */
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
		Config(std::string FileName);
		void		Init();

		void		Parse();
		void		Tokenise(const std::string& LineStr);

		void		AssignTokens(TokenLine& LineTokens, std::string& Scope);
		void		AssignGlobalParams(Token& Key, std::vector<Token>& Tokens);
		void		AssignServer(Token& Key, std::vector<Token>& Tokens, std::string& Scope);
		void		AssignLocation(Token& Key, std::vector<Token>& Tokens, std::string& Scope);

		ConfigLines	GetLines();
		ConfigFile	GetResult();

	private:
		std::map<std::string, int>	Keys;
		ConfigFile					Result;
		std::fstream				File;
		ConfigLines					ConfLines;
};

std::ostream&	operator<<(std::ostream& o, ConfigLines& c);
std::ostream&	operator<<(std::ostream& o, Config& c);
std::ostream&	operator <<(std::ostream& o, ServerConf& s);
std::ostream&	operator<<(std::ostream& o, RouteConf& r);


#endif