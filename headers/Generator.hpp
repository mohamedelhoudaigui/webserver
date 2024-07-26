/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Generator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 01:32:08 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/26 11:26:34 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "Utils.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

#define ERR_FILE "./www/error_template.html"

class	Generator
{
	public:
		Generator(std::string& err, int status_code, std::string file);
		std::string		body;
		~Generator();
};



#endif