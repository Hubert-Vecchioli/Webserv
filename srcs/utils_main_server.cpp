/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main_server.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 17:31:44 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/27 17:48:11 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server ft_main_server(int mode, Server & new_main_server)
{
	static Server main_server;

	if (mode == 1)
		main_server  = new_main_server;
	return (main_server);
}

Server	ft_get_main_server(void)
{
	return (ft_main_server(0, NULL));
}

void	ft_set_main_server(Server & new_main_server)
{
	ft_main_server(1, new_main_server);
	return ;
}