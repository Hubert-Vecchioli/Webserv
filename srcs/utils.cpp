/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 13:14:32 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/08 07:12:33 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	displayTimestamp(void)
{
	std::time_t now = time(0);
    std::tm *date = localtime(&now);
	std::cout << "[";
	std::cout << std::setfill('0') << 1900 + date->tm_year;
	std::cout << " - ";
	std::cout << std::setfill('0') << std::setw(2) << 1 + date->tm_mon;
	std::cout << " - ";
	std::cout << std::setfill('0') << std::setw(2) << date->tm_mday;
	std::cout << " ";
	std::cout << std::setfill('0') << std::setw(2) << date->tm_hour;
	std::cout << " : ";
	std::cout << std::setfill('0') << std::setw(2) << date->tm_min;
	std::cout << " : ";
	std::cout << std::setfill('0') << std::setw(2) << date->tm_sec;
	std::cout << "] ";
}

void	print(int i, std::string message)
{
	displayTimestamp();
	if(i == 1)
		std::cout << message << std::endl;
	else
		std::cerr << message << std::endl;
}

void	modifyEpollCTL(int EpollFD, int listendFD, int epollAction, bool isReadyToSendResponse = false)
{
	epoll_event ev;
	if (epollAction != EPOLL_CTL_DEL)
		ev.events = EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN;
	if (epollAction == EPOLL_CTL_MOD && isReadyToSendResponse)
		ev.events = EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN;
	ev.data.fd = listendFD;
	if (epoll_ctl(EpollFD, epollAction, listendFD, &ev) == -1)
		throw Server::FailureModifyFDEpollException();
}
