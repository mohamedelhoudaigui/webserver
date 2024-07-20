/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Generator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 01:32:08 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/20 08:25:34 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "../utils/utils.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

class	Generator
{
	public:
		Generator(std::string& err, int status_code);
		std::ifstream	html_temp;
		std::string		body;
		~Generator();
};



#endif