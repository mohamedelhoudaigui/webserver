/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Generator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theworld27 <theworld27@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 01:34:30 by mel-houd          #+#    #+#             */
/*   Updated: 2024/09/02 08:56:39 by theworld27       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Generator.hpp"

Generator::Generator(std::string& err, int status_code, std::string file_path)
{
	std::ostringstream	file;
	std::ifstream	html_temp;

	if (status_code != 200)
		html_temp.open(ERR_FILE);
	else
		html_temp.open(file_path.c_str());
	if (!html_temp.is_open())
	{
		std::cerr << "error reading html template\n";
		exit(1);
	}
    file << html_temp.rdbuf();
	std::string	result = file.str();
	if (status_code != 200)
	{
		size_t	pos;
		while ((pos = result.find("{TITLE}")) != std::string::npos)
		{
			std::string	num = itos(status_code);
	        result.replace(pos, 7, num);
	    }
	    while ((pos = result.find("{MAIN}")) != std::string::npos)
		{
			std::string	line(err + " " + itos(status_code));
	        result.replace(pos, 6, line);
	    }
	}
	this->body = result;
}

Generator::~Generator()
{
}
