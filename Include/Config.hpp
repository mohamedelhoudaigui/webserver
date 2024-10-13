/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 02:49:00 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 09:46:44 by mel-houd         ###   ########.fr       */
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
		ConfigLines	GetLines();
		ConfigFile	GetResult();
		void		Tokenise(const std::string& LineStr);
		void		AssignTokens(TokenLine& LineTokens);
		void		AssignGlobalParams(Token& Key, std::vector<Token>& Tokens);
		void		AssignServer(Token& Key, std::vector<Token>& Tokens);

	private:
		std::map<std::string, int>	Keys;
		ConfigFile					Result;
		std::fstream				File;
		ConfigLines					ConfLines;
};

std::ostream&	operator<<(std::ostream& o, ConfigLines& c);


#endif