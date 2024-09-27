/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:18 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/27 17:15:30 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include <map>

class HttpRequest
{
private:

public:
	HttpRequest(void);
	HttpRequest(HttpRequest const & rhs);
	HttpRequest &operator=(HttpRequest const & rhs);
	~HttpRequest(void);
	
};

#endif