/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 17:41:09 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/27 17:49:24 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils_main_server.cpp"
#include "utils.cpp"
#include "iostream"

void ft_stop_server(int signum)
{
	displayTimestamp();
	std::cout << "[Info] - Signal request to stop the server received" << std::endl;
	ft_get_main_server().stop();
}