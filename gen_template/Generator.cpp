/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Generator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 01:34:30 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/20 08:25:34 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Generator.hpp"

Generator::Generator(std::string& err, int status_code) : html_temp("./www/error_template.html")
{
	if (!this->html_temp)
	{
		std::cerr << "error reading html template\n";
		exit(1);
	}
	std::ostringstream	file;
    file << this->html_temp.rdbuf();
	std::string	result = file.str();
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
	this->body = result;
}

Generator::~Generator()
{
}
