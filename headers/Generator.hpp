/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Generator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theworld27 <theworld27@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 01:32:08 by mel-houd          #+#    #+#             */
/*   Updated: 2024/09/02 08:56:55 by theworld27       ###   ########.fr       */
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
#include <cstdlib>

#define ERR_FILE "./www/error_template.html"

class	Generator
{
	public:
		Generator(std::string& err, int status_code, std::string file);
		std::string		body;
		~Generator();
};



#endif