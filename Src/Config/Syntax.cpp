/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Syntax.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:42:17 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/15 01:20:47 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"


void	Config::CheckScopeKeyWord(std::string& Key, std::vector<std::string>& Keys)
{
	std::vector<std::string>::iterator it = std::find(GlobalKeys.begin(), GlobalKeys.end(), Key);
	if (it == GlobalKeys.end())
		throw std::runtime_error(Key + ": Key word doesnt belong to scope");
}


void	Config::AssignLine(Token& key, std::vector<Token>& Tokens)
{
	//
}

void	Config::CheckSyntaxError()
{
	Scope s = GLOBAL;
	for (std::vector<TokenLine>::iterator it = ConfLines.TokenLines.begin(); it != ConfLines.TokenLines.end(); ++it)
	{
		Token key = it->Tokens[0];

		if (key.Token == "Server")
		{
			if (s != GLOBAL)
				throw std::runtime_error("Invalid Server Scope");
			if (it->Tokens.size() != 2 || it->Tokens[1].Type != OPEN)
				throw std::runtime_error("Server scope with no open closure");
			s = SERVER;
		}
		else if (key.Token == "Location")
		{
			if (s != SERVER)
				throw std::runtime_error("Invalid Location Scope");
			if (it->Tokens.size() != 2 || it->Tokens[1].Type != OPEN)
				throw std::runtime_error("Loctaion scope with no open closure");
			s = LOCATION;
		}

		else if (key.Type = CLOSE)
		{
			if (s == GLOBAL)
				throw std::runtime_error("Closure in global scope");
			else if (s == SERVER)
				s = GLOBAL;
			else if (s == LOCATION)
				s = SERVER;
		}
		AssignLigne(key, Tokens);
	}
}