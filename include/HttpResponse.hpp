/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:00 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/08 19:14:11 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include "webserv.hpp"

class HttpResponse
{
	private:
		bool		_isResponseSent;
		std::string	_reponseContent;
		std::time_t _lastActionTimeStamp;
		void 		_generateResponseContent(void);
		void 		_generatePOSTResponse(void);
		void 		_generatePUTResponse(void);
		void 		_generateDELResponse(void);
		void 		_generateGETResponse(void);
		void 		_generateInvalidResponse(int code);

	public:
		HttpResponse(void);
		HttpResponse(HttpResponse const & rhs);
		HttpResponse &operator=(HttpResponse const & rhs);
		~HttpResponse(void);
		
		bool getResponseStatus(void) const {return _isResponseSent};
		std::time_t getLastActionTimeStamp(void) const {return _lastActionTimeStamp};
		std::string getResponseContent(void);
};

#endif