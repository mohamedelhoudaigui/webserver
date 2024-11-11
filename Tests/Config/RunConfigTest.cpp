/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RunConfigTest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:30:04 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/11 17:07:57 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"


static void	TestUnit(std::string FileName, std::string Note)
{
	try {
		Config c(FileName);
		c.Init();
		c.Parse();
	} catch (const std::exception& e) {
		std::cout << Note << e.what() << std::endl;
	}
}

void	RunTests()
{
	std::string	TestsPath = "./Tests/Config/Files/";

	TestUnit(TestsPath + "valid", "valid file exception : "); // valid test should not throw exception
	for (int i = 1; i < 8; ++i) // invalid config files
	{
		std::stringstream	ss;
		ss << i;
		TestUnit(TestsPath + ss.str(), ss.str() + " good : ");
	}
}