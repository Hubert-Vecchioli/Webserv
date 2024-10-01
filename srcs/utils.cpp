/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 13:14:32 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/01 12:17:54 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	displayTimestamp(void)
{
	std::time_t now = time(0);
    std::tm *date = localtime(&now);
	std::cout << "[";
	std::cout << std::setfill('0') << 1900 + date->tm_year;
	std::cout << std::setfill('0') << std::setw(2) << 1 + date->tm_mon;
	std::cout << std::setfill('0') << std::setw(2) << date->tm_mday;
	std::cout << "_";
	std::cout << std::setfill('0') << std::setw(2) << date->tm_hour;
	std::cout << std::setfill('0') << std::setw(2) << date->tm_min;
	std::cout << std::setfill('0') << std::setw(2) << date->tm_sec;
	std::cout << "] ";
}

void	print(int i, std::string message)
{
	displayTimestamp();
	if(i == 1)
		std::cout << message << std::endl;
	else
		std::cerr << message << std::endl;
}
