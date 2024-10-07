/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:00 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/07 15:16:03 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include "webserv.hpp"

class HttpResponse
{
	private:
		bool				_isResponseSent;

	public:
		HttpResponse(void);
		HttpResponse(HttpResponse const & rhs);
		HttpResponse &operator=(HttpResponse const & rhs);
		~HttpResponse(void);
		
		bool getResponseStatus(void) {return _isResponseSent};
};

#endif