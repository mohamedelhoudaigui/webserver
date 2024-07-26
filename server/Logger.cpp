/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 05:13:01 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/26 11:21:46 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Logger.hpp"

Logger::Logger(std::string filename)
{
    log.open(filename, std::ios::out);
    if (!log.is_open())
	{
        std::cerr << "Failed to open log file" << std::endl;
    }
}

Logger::~Logger()
{
    if (log.is_open())
	{
        log.close();
    }
}

void	Logger::write(std::string message)
{
    if (log.is_open())
	{
        log << message << std::endl;
		log  << "--------------------------------\n";
        log.flush(); // Ensure the data is written immediately
    }
}