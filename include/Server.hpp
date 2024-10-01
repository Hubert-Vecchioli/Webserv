/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:07 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/01 08:45:51 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "webserv.hpp"



class Server
{
	private:
		bool _isServerGreenlighted;
		int _serverFD;
		std::map<int, Socket*>	_sockets;
		std::map<int, Client*>	_clients;

	public:
		Server(void);
		Server(Server const & rhs);
		Server &operator=(Server const & rhs);
		~Server(void);
		
		void initate(ConfigurationFile & config);
		void run(void);
		void stop(void);
};

#endif