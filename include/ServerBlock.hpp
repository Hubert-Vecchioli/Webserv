/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:07:40 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/05 15:30:49 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include "webserv.hpp"

class ServerBlock
{
	private:
		std::pair<std::string, int>	_listen;
		std::string 	_server_name;
		std::vector<LocationBlock> _locationBlocks;

		void parseListen(std::string block);  //get IP and Port -> listen
		void parseServerName(std::string block); //get server name
		void parseLocationBlock(std::string block); //get location blocks in the server block
		void parseIPandPort(std::string block); //parse IP annd Port via regex
		

	public:
		ServerBlock(std::string block);
		ServerBlock(ServerBlock const & rhs);
		ServerBlock &operator=(ServerBlock const & rhs);
		~ServerBlock(void);

		std::pair<string, int> getIPandPort(void) const;
};

#endif
