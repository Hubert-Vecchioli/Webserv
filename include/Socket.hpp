/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:53:56 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/27 17:14:51 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <map>

class Socket
{
private:

public:
	Socket(void);
	Socket(Socket const & rhs);
	Socket &operator=(Socket const & rhs);
	~Socket(void);
	
};

#endif