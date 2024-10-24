/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Excpetions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 01:05:13 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/24 01:06:20 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include "Types.hpp"


class GetterExc : public std::exception
{
	public:
		GetterExc(Error err);
		Error err;
};


#endif