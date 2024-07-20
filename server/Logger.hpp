/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 05:13:13 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/20 05:18:19 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>

class	Logger
{
	public:
		std::ofstream log;
		Logger(std::string filename);
		~Logger();
		void write(std::string message);
};


#endif