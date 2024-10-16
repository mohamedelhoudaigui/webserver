/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:51:31 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/16 18:56:45 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

ConfigLines	Config::GetLines()
{
	return (this->ConfLines);
}

ConfigFile	Config::GetResult()
{
	return (this->Result);
}

//GlobalGetters :
std::string&	Config::GetDefaultErrorPage()
{
	return (Result.DefaultErrorPage);
}

//ServerGetters :
