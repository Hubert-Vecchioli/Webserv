/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:07:40 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/09 01:29:49 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include "webserv.hpp"

class LocationBlock;

class ServerBlock
{
	private:
		std::pair<std::string, int>	_listen;
		std::vector<std::string>	_server_name;
		std::map<int, std::string> _error_pages;
		std::vector<LocationBlock> _locationBlocks;

		void parseListen(std::string block);  //get IP and Port -> listen
		void parseServerName(std::string block); //get server name
		void parseErrorPages(std::string block); //get error pages
		void parseLocationBlock(std::string block); //get location blocks in the server block
		

	public:
		ServerBlock(std::string block);
		ServerBlock(ServerBlock const & rhs);
		ServerBlock &operator=(ServerBlock const & rhs);
		~ServerBlock(void);

		std::pair<std::string, int> getIPandPort(void) const;
		std::vector<std::string> getServerName(void) const;
		std::map<int, std::string> getErrorPages(void) const;
		std::vector<LocationBlock> getLocationBlocks(void) const;
};

#endif
