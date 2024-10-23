/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:23:30 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/23 13:36:50 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int ac, char **av)
{
	Server &server  = Server::getInstance();

	// server->getInstance();
	signal(SIGINT, ft_stop_server);
	try
	{
		ConfigurationFile config = ConfigurationFile(ac, av);// load & parse a config file OR load default config - and its respective cout message
		std::vector<std::pair<std::string, int> > iphost = config.getserverIPandPorts();
		for (unsigned long i = 0; i < iphost.size(); i++)
		{
			std::cout << iphost[i].first << " " << iphost[i].second << std::endl;
		}
		server.startServer(&config); //Should I add the config object as an argument of initiate?
		server.runServer();
	}
	catch (std::exception & e)
	{
		print(2, e.what());
		return (1);
	}
	print(1, "[Info] - Webserv is now stopped");
	return (0);
}
