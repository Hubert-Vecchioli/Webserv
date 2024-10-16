/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 17:41:09 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/16 16:52:49 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void ft_stop_server(int signum)
{
	(void) signum;
	print(1, "[Info] - Signal request to stop the server received");
	Server::getInstance().stopServer();
}