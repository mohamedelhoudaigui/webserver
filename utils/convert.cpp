/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 01:26:22 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/20 06:14:58 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string itos(int value)
{
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}

std::string	ctos(char *buffer)
{
	std::string	buf(buffer);
	return (buf);
}
