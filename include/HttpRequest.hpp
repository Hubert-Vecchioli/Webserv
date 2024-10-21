/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuteness_ <cuteness_@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:18 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/21 15:21:18 by cuteness_        ###   ########.fr       */
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
		std::vector<std::string>			_accept; //header
		t_connection						_connection; //header
		std::string							_content_type; //header
		size_t								_content_len; //header
		std::map<std::string, std::string>	_cookie; //header
		std::string							_content_body; //body
		
		std::string getValue(std::string request, std::string key);
		void parseRequestLine(std::string request);
		void parseRequestHeader(std::string request);
    	void parseRequestBody(std::string request);
		void parseConnection(std::string request);
		void parseCookie(std::string str_request);

	public:
		HttpRequest(Client *client, HttpResponse *response, char *request);
		HttpRequest(HttpRequest const & rhs);
		HttpRequest &operator=(HttpRequest const & rhs);
		~HttpRequest(void);

		void displayRequestLine(std::ostream & o);
		void displayRequestHeader(std::ostream & o);
		void displayRequestBody(std::ostream & o);

		HttpResponse* getResponse(void) {return _response;};
		static HttpRequest* findInstanceWithFD(std::vector<HttpRequest*>& vector, int fd);

		t_method getMethod() {return _method;};
		bool getHTTP() {return _http1;};
		std::string getQueryString() {return _queryString;};
		std::string getRequestURI() {return _requestURI;};
		std::string getHost() {return _host;};
		std::vector<std::string> getAccept() {return _accept;};
};

#endif
