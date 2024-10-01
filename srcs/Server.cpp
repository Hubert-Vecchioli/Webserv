/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:05 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/01 13:08:31 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Server::Server() : _isServerGreenlighted(false), _serverFD(-1)
{
}

Server::~Server()
{
	//close FD and delete associated containers
}

void Server::init(void)
{
	print(1, "[Info] - Initiating the webserv");
	this->_serverFD(epoll_create(MAX_EVENTS));
    if (this->_serverFD == -1)
		print(2, "[Error] - Failure to initiate the epoll instance");
        // Should we stop the project ?
	// Request a socket FD for each port to listen to - thanks to the function socket()?
	// Return the FD and place it under watch of epoll thanks to epoll_ctl
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