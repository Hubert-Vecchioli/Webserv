/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:05 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/02 17:09:05 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Server::Server() : _isServerGreenlighted(false), _serverFD(-1)
{
}

Server::~Server()
{
	if (this->_serverFD != -1)
		close(_serverFD);
	
	//close FD and delete associated containers
}

void Server::init(void)
{
	print(1, "[Info] - Initiating the webserv");
	this->_serverFD(epoll_create(MAX_EVENTS));
    if (this->_serverFD == -1)
		print(2, "[Error] - Failure to initiate the epoll instance");
        // Should we stop the project ?
	std::vector<std::vector<BlockServer>> &parsed_config = this->_config.getBlockServers();
	for (std::vector<BlocServer>::iterator it = parsed_config.begin(); it != parsed_config.end(); ++it)
	{
		int blocServersFD = socket(AF_INET, SOCK_STREAM, 0);
		if (blocServersFD == -1)
			print(2, "[Error] - Failure to initiate a socket while initialising servers");
			// Should we stop the project ?
		epoll_event ev;
		ev.events = EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR ;
		ev.data.fd = blocServersFD;
		int epollFDAddResult = epoll_ctl(this->_serverFD, EPOLL_CTL_ADD, blocServersFD, &ev)
		if (epollFDAddResult == -1)
			print(2, "[Error] - Failure to add the FD to the epoll listen list");
			// Should we stop the project ?
		this->_sockets[blocServersFD] = new Socket(blocServersFD, &it->second);
	}
	this->_isServerGreenlighted = true;
}

void Server::run(void)
{
	
	while(this->_isServerGreenlighted == true)
	{
		//epoll_wait
	}
}

void Server::stop( void )
{
	this->_isServerGreenlighted(false);
}