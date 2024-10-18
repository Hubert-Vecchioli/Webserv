/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 17:43:06 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/18 11:50:21 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <vector>
# include <iomanip>
# include <ctime>
# include <string>
# include <cstdlib>
# include <limits>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/stat.h>

# include "Server.hpp"
# include "Socket.hpp"
# include "Client.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ConfigurationFile.hpp"
# include "Client.hpp"
# include "LocationBlock.hpp"
# include "ServerBlock.hpp"

//# include "signal.cpp"



#define MAX_EVENTS 50 // to be increassed later on
#define EPOLL_MAX_WAIT_TIME_MS 1000 // to be modified later on
#define CLIENT_TIMEOUT_LIMIT_SEC 10
#define REQUEST_TIMEOUT_LIMIT_SEC 10
#define MAX_REQUEST_SIZE 5120
#define	MAX_URI_SIZE 65536

void	displayTimestamp(void);
void	print(int i, std::string message, int fd = -2);
std::vector<std::string> tokenize(std::string s, char delimiter);
void	modifyEpollCTL(int EpollFD, int listendFD, int epollAction, bool isReadyToSendResponse = false);

#endif
