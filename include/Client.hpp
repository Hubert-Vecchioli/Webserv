/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:05 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/06 10:00:42 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "webserv.hpp"

class Client
{
	private:
		std::time_t _lastActionTimeStamp;
	public:
		Client(void);
		Client(Client const & rhs);
		Client &operator=(Client const & rhs);
		~Client(void);
};

#endif
