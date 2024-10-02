/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:53:54 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/02 17:06:41 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Socket::Socket(void) : _fd(-1)
{
}

Socket::Socket(int fd, std::string ip, unsigned int port, std::vector<BlocServer>* servers)
{
}

Socket::~Socket(void)
{
	if (this->_fd != -1)
		close(this->_fd); // should it be protected?
}
