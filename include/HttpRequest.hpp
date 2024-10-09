/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:18 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/09 18:33:42 by jblaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "webserv.hpp"

typedef enum e_method {
	GET,
	POST,
	DELETE,
	ERROR
}				t_method;

typedef enum e_connection {
	KEEP_ALIVE,
	CLOSE
}				t_connection;

class HttpRequest
{
	private:
		Client*								_client;
		HttpResponse*						_response;
		t_method							_method;
		bool								_http1;
		std::string							_queryString;
		std::string							_requestURI;
		std::string							_HTTPVersion;
		std::string							_host;
		t_connection						_connection;
		std::string							_content_type;
		size_t								_content_len;
		std::string							_content_body;
		std::string							_accept;

	public:
		HttpRequest(char *request);
		HttpRequest(HttpRequest const & rhs);
		HttpRequest &operator=(HttpRequest const & rhs);
		~HttpRequest(void);

		parseHeader(char *request);
		parseRequestLine(char *request);
    	parseBody(char *request);

		HttpResponse* getResponse(void) {return _response};
		static HttpRequest* findInstanceWithFD(std::vector<HttpRequest>& vector, int fd);

};

#endif