/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokens.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:30:30 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/14 18:49:53 by mel-houd         ###   ########.fr       */
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
			TokeniseLine(TrimAll(Buffer));
		}
	}
	File.close();
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


void	Config::AssignTokens()
{
	for (std::vector<TokenLine>::iterator it = ConfLines.TokenLines.begin();  it != ConfLines.TokenLines.end(); ++it)
	{
		std::vector<Token>	Tokens = it->Tokens;
		size_t				Ntokens = Tokens.size();

		Token Key = Tokens[0];
		AssignGlobalParams(Key, Tokens);
		AssignServer(Key, Tokens);
		AssignLocation(Key, Tokens);
	}
}
