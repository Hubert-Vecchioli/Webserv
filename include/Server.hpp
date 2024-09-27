/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:07 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/27 17:14:55 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>
//# include <sys/epoll.h>
# include "Socket.hpp"
# include "Client.hpp"

class Server
{
private:
	bool isServerGreenlighted;
	int serverFD;
	std::map<int, Socket*>	_sockets;
	std::map<int, Client*>	_clients;

public:
	Server(void);
	Server(Server const & rhs);
	Server &operator=(Server const & rhs);
	~Server(void);
	
	void initate(void);
	void run(void);
	void stop(void);
	
};

#endif