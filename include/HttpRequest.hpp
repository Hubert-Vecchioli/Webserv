/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:18 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/07 15:17:00 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "webserv.hpp"

class HttpRequest
{
	private:
		Client*								_client;
		HttpResponse*						_response;

	public:
		HttpRequest(void);
		HttpRequest(HttpRequest const & rhs);
		HttpRequest &operator=(HttpRequest const & rhs);
		~HttpRequest(void);

		HttpResponse* getResponse(void) {return _response};
};

#endif