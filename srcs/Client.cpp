/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:53:51 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/06 15:02:15 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Client::Client() : _lastActionTimeStamp(std::time(nullptr))
{
	
}

int Client::getFD(void)
{
	return (_fd);
}

Client* Client::findInstanceWithFD(std::vector<Client>& vector, int fd) {
    for (std::vector<Client>::iterator it = vector.begin(); it != vector.end(); ++it) {
        if (it->_fd == fd) {
            return &(*it);
        }
    }
    return (nullptr);
}