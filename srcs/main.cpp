/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:23:30 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/27 17:42:31 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "signal.cpp"

int main(int ac, char **av)
{
	Server server;
	ft_set_main_server(server);
	
	signal(SIGINT, ft_stop_server);
}