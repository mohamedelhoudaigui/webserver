/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Syntax.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:42:17 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/15 18:35:58 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"


void	Config::CheckScopeKeyWord(std::string& Key, std::vector<std::string>& Keys)
{
	std::vector<std::string>::iterator it = std::find(Keys.begin(), Keys.end(), Key);
	if (it == Keys.end())
		throw std::runtime_error(Key + ": Key word doesnt belong to scope");
}


void	Config::CheckScope(Token& Key, std::vector<Token>& Tokens, Scope& s)
{
	switch (s)
	{
		case GLOBAL:
			CheckScopeKeyWord(Key.Token, this->GlobalKeys);
			break;

		case SERVER:
			CheckScopeKeyWord(Key.Token, this->ServerKeys);
			break ;
		
		case LOCATION:
			CheckScopeKeyWord(Key.Token, this->LocationKeys);
			break;
	}
}

void	Config::CheckSyntaxError()
{
	Scope s = GLOBAL;
	for (std::vector<TokenLine>::iterator it = ConfLines.TokenLines.begin(); it != ConfLines.TokenLines.end(); ++it)
	{
		Token Key = it->Tokens[0];

		if (Key.Token == "Server")
		{
			if (s != GLOBAL)
				throw std::runtime_error("Invalid Server Scope");
			if (it->Tokens.size() != 2 || it->Tokens[1].Type != OPEN)
				throw std::runtime_error("Server scope with no open closure");
			s = SERVER;
		}

		else if (Key.Token == "Location")
		{
			if (s != SERVER)
				throw std::runtime_error("Invalid Location Scope");
			if (it->Tokens.size() != 3 || it->Tokens[2].Type != OPEN)
				throw std::runtime_error("Loctaion scope with no open closure");
			s = LOCATION;
		}

		else if (Key.Type == CLOSE)
		{
			if (s == GLOBAL)
				throw std::runtime_error("Closure in global scope");
			else if (s == SERVER)
				s = GLOBAL;
			else if (s == LOCATION)
				s = SERVER;
		}
		else
			CheckScope(Key, it->Tokens, s);
	}
	if (s != GLOBAL)
		throw std::runtime_error("Closure tag not found !");
}