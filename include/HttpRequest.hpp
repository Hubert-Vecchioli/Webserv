/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:18 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/12 18:36:41 by jblaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "webserv.hpp"

class HttpResponse;

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
		t_method							_method; //requestLine
		bool								_http1; //requestLine
		std::string							_queryString; //requestLine
		std::string							_requestURI; //requestLine
		std::string							_host; //header
		std::string							_accept; //header
		t_connection						_connection; //header
		std::string							_content_type; //header
		size_t								_content_len; //header
		std::string							_content_body; //body
		
		std::string getValue(std::string request, std::string key);
		void parseRequestLine(char *request);
		void parseRequestHeader(char *request);
    	void parseRequestBody(char *request);

	public:
		HttpRequest(Client *client, HttpResponse *response, char *request);
		HttpRequest(HttpRequest const & rhs);
		HttpRequest &operator=(HttpRequest const & rhs);
		~HttpRequest(void);

		void displayRequestLine(std::ostream & o);
		void displayRequestHeader(std::ostream & o);
		void displayRequestBody(std::ostream & o);

		HttpResponse* getResponse(void) {return _response;};
		static HttpRequest* findInstanceWithFD(std::vector<HttpRequest>& vector, int fd);
};

#endif