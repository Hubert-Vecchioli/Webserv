/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:53:54 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/06 15:17:13 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Socket::Socket(void) : _fd(-1)
{
}

Socket::Socket(int fd, unsigned int port, std::string ip) : _fd(fd), _port(port), _ip(ip)
{
	try
	{
		this->_sockaddr.sin_port = htons(this->_port);
		this->_sockaddr.sin_family = AF_INET;
		this->_sockaddr.sin_addr.s_addr = inet_addr(this->_ip.c_str());
		if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) == -1)
			throw FailureSetNonBlockingSocketException();
		int validateFlag = 1;
		if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &validateFlag, sizeof(int)) == -1)
			throw FailureUpdateSocketException();
		if (bind(this->_fd, (struct sockaddr *)&this->_sockaddr, sizeof(this->_sockaddr)) == -1)
			throw FailureBindSocketException();
		if (listen(this->_fd, 100) == -1) 
			// this 
			// 100 here is the max number of incoming connections that can be queued up while the server is busy handling other connections
			throw FailureSocketListenException();
	}
	catch (std::exception &e)
	{
		print(2, e.what());
		if (this->_fd != -1)
			close(this->_fd);
	}
	print(1, "[Info] - Socket created and well set-up");
}

Socket* Socket::findInstanceWithFD(std::vector<Socket>& vector, int fd) {
    for (std::vector<Socket>::iterator it = vector.begin(); it != vector.end(); ++it) {
        if (it->_fd == fd) {
            return &(*it);
        }
    }
    return (nullptr);
}

Socket::~Socket(void)
{
	if (this->_fd != -1)
		close(this->_fd); // should it be protected?
}

const char* Socket::FailureSetNonBlockingSocketException::what() const throw()
{
	return ("[Error] - Failure to set the socket to non-blocking");
}

const char* Socket::FailureUpdateSocketException::what() const throw()
{
	return ("[Error] - Failure to update the socket options (to allow other fd to listen to same address)");
}

const char* Socket::FailureBindSocketException::what() const throw()
{
	return ("[Error] - Failure to bind the socket");
}
const char* Socket::FailureSocketListenException::what() const throw()
{
	return ("[Error] - Failure to listen to the socket");
}