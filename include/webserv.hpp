/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 17:43:06 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/06 09:57:11 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "Server.hpp"
# include "Socket.hpp"
# include "Client.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ConfigurationFile.hpp"
# include "Client.hpp"
# include "ServerBlock.hpp"
# include "utils.cpp"
# include "signal.cpp"

# include <iostream>
# include <map>
# include <iomanip>
# include <ctime>
# include <string>
# include <cstdlib>
# include <limits>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <fcntl.h>

#define MAX_EVENTS 50 // to be increassed later on
#define EPOLL_MAX_WAIT_TIME_MS 1000 // to be modified later on

#endif