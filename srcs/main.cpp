/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:23:30 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/23 14:58:27 by hvecchio         ###   ########.fr       */
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
		
		unsigned long body = config.getBody_size();
		std::string user = config.getUser();
		std::string error = config.getError_log();
		std::cout << body << " " << user << " " << error << std::endl;
		
		std::vector<ServerBlock> sblock	= config.getServerBlocks();
		for (size_t i = 0; i < sblock.size(); i++)
		{
			std::cout << "--------- SERVER BLOCKS ----------" << std::endl;
			std::pair<std::string, int> ipport = sblock[i].getIPandPort();
			std::map<int, std::string> error = sblock[i].getErrorPages();
			std::vector<std::string> name = sblock[i].getServerName();
			
			std::cout <<" IP and Port: " << ipport.first << " " << ipport.second << std::endl;
			for (size_t j = 0; j < name.size(); j++)
				std::cout << "Server Name: " << name[j] << " ";
			std::cout << std::endl;
			for (std::map<int, std::string>::iterator it = error.begin(); it != error.end(); it++)
				std::cout << "Error Pages: " << it->first << " " << it->second << std::endl;
			std::vector<LocationBlock> lblock = sblock[i].getLocationBlocks();
			for (size_t j = 0; j < lblock.size(); j++)
			{
				std::cout << "------LOCATION BLOCKS------" << std::endl;
				std::string location = lblock[j].getLocation();
				std::string root = lblock[j].getRoot();
				std::vector<std::string> index = lblock[j].getIndex();
				bool dir = lblock[j].getDirlisting();
				std::vector<std::string> methods = lblock[j].getMethods();
				std::pair<int, std::string> redirect = lblock[j].getRedirect();
				std::map<std::string, std::string> cgi = lblock[j].getCgiExtension();
				std::string upload = lblock[j].getUploadPath();

				std::cout << "Location: " << location << " - Root: " << root << " - Dirlisting: " << dir << " - UploadDir: " << upload << std::endl;
				for (size_t k = 0; k < index.size(); k++)
					std::cout << "Index: " << index[k] << " ";
				std::cout << std::endl;
				for (size_t k = 0; k < methods.size(); k++)
					std::cout << "Http Methods: " << methods[k] << " ";
				std::cout << std::endl;
				std::cout << "Redirect info: " << redirect.first << " " << redirect.second << std::endl;
				for (std::map<std::string, std::string>::iterator it = cgi.begin(); it != cgi.end(); it++)
					std::cout << "CGI extensions: " << it->first << " " << it->second << std::endl;
			}
		}
		std::cout << "-------------------" << std::endl;
		server.startServer(config); //Should I add the config object as an argument of initiate?
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
