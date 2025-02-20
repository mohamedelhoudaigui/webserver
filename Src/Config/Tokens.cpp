/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokens.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:30:30 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/15 19:45:55 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"


void	Config::Tokeniser()
{
	std::string	Buffer;

	while (std::getline(File, Buffer))  // tokenise and fills this->ConfLines struct
	{
		Buffer = TrimAll(Buffer);
		if (Buffer != "" && Buffer[0] != '#')
		{
			TokeniseLine(Buffer);
		}
	}
	File.close();
	Logger(DEBUG, "tokenising process finished");
}

void	Config::TokeniseLine(const std::string& LineStr) // tokenizer
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
            token.Token = Buffer;

			if (Buffer == "}") // close Token
				token.Type = CLOSE;
			else if (Buffer == "{") // open Token
				token.Type = OPEN;

			else if (Key && Buffer != "}" && Buffer != "{") // key Token
			{
				token.Type = KEY;
				Key = false;
			}
			else
				token.Type = VALUE;

			Line.Tokens.push_back(token);
		}
	}

	ConfLines.TokenLines.push_back(Line);
}

void	Config::CheckScopeKeyWord(std::string& Key, std::vector<std::string>& Keys)
{
	std::vector<std::string>::iterator it = std::find(Keys.begin(), Keys.end(), Key);
	if (it == Keys.end())
		throw std::runtime_error(Key + ": Key word doesnt belong to scope");
}


void	Config::CheckScope(std::vector<Token>& Tokens, Scope& s)
{
    Token& Key = Tokens[0];

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
    std::vector<TokenLine>::iterator it;

	for (it = ConfLines.TokenLines.begin(); it != ConfLines.TokenLines.end(); ++it)
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
			CheckScope(it->Tokens, s);
	}
	if (s != GLOBAL)
		throw std::runtime_error("Closure tag not found !");
	Logger(DEBUG, "syntax error checked");
}
