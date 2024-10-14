/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RunConfigTest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:30:04 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/14 11:49:34 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

void	RunTests()
{
	try{
		Config c("./Tests/Config/3"); // -- no closing brackets need error
		c.Init();
		c.Parse();
	}
	catch (const std::exception& e)
	{
		std::cout << "Testing Exception:" << std::endl;
		std::cout << e.what() << std::endl;
	}
	std::cout << "All tests passed" << std::endl;
}