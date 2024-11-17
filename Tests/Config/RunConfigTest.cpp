/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RunConfigTest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:30:04 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/17 10:08:58 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"


static void	UnitTest(std::string FileName, std::string Note)
{
	try {
		Config c(FileName);
		c.Parse();
	} catch (const std::exception& e) {
		std::cout << Note << e.what() << std::endl;
	}
}

void	TestConfig()
{
	std::string	TestsPath = "./Tests/Config/Files/";

	for (int i = 1; i < 8; ++i) // invalid config files
	{
		std::stringstream	ss;
		ss << i;
		UnitTest(TestsPath + ss.str(), ss.str() + " good : ");
	}
}