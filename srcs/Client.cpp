/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:53:51 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/12 18:32:06 by jblaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Client::Client(void): _lastActionTimeStamp(time(0)), _fd(-1)
{
}

Client::Client(int fd) : _lastActionTimeStamp(time(0)), _fd(fd)
{
}

Client::~Client(void)
{
}

Client::Client(Client const & rhs)
{
	*this = rhs;
	return ;
}

Client & Client::operator=(Client const & rhs)
{
	if (this != &rhs)
	{
		this->_lastActionTimeStamp = rhs._lastActionTimeStamp;
		this->_fd = rhs._fd;
	}
	return (*this);
}

int Client::getFD(void)
{
	return (this->_fd);
}

void Client::updateLastActionTimeStamp(void)
{
	this->_lastActionTimeStamp = time(0);
}

Client* Client::findInstanceWithFD(std::vector<Client>& vector, int fd) {
    for (std::vector<Client>::iterator it = vector.begin(); it != vector.end(); ++it) {
        if (it->_fd == fd) {
            return &(*it);
        }
    }
    return (0);
}