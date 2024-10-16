/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:00 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/16 14:40:08 by jblaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include "webserv.hpp"

class HttpResponse
{
	private:
		Server			&_server;
		bool			_isResponseSent;
		std::string		_reponseContent;
		std::time_t 	_lastActionTimeStamp;
		void 			_generateResponseContent(HttpRequest & request);
		void 			_generateGETResponse(HttpRequest & request);
		void 			_generatePOSTResponse(void);
		void 			_generateDELResponse(void);
		void 			_generateInvalidMethod(void);
		ServerBlock		&_fetchServerBlock(HttpRequest & request);
		LocationBlock	&_fetchLocationBlock(HttpRequest & request, ServerBlock & server_block);
		

	public:
		HttpResponse(void);
		HttpResponse(HttpResponse const & rhs);
		HttpResponse &operator=(HttpResponse const & rhs);
		~HttpResponse(void);
		
		bool getResponseStatus(void) const {return _isResponseSent;};
		std::time_t getLastActionTimeStamp(void) const {return _lastActionTimeStamp;};
		std::string getResponseContent(void);

		class ServerError : public std::exception {
			private:
				int							_errorCode;
				std::map<int, std::string>	_errorMap;
			public:
				ServerError(int code): _errorCode(code) {
					_errorMap[500] = "Internal Server Error\n";
					_errorCode[501] = "Not Implemented\n";
					_errorCode[502] = "Bad Gateway\n";
					_errorCode[503] = "Service Unavailable\n";
					_errorCode[504] = "Gateway Timeout\n";
					_errorCode[505] = "HTTP Version Not Supported\n";
				}
				virtual const char *what() const throw();
		};

		class ClientError : public std::exception {
			private:
				int							_errorCode;
				std::map<int, std::string>	_errorMap;
			public:
				ClientError(int code): _errorCode(code) {
					_errorCode[400] = "Bad Request\n";
					_errorCode[401] = "Unauthorized\n";
					_errorCode[403] = "Forbidden\n";
					_errorCode[404] = "Not Found\n";
					_errorCode[405] = "Method Not Allowed\n";
					_errorCode[406] = "Not Acceptable\n";
					_errorCode[407] = "Proxy Authentication Required\n"
					_errorCode[408] = "Request Timeout\n";;
					_errorCode[409] = "Conflict\n";
					_errorCode[410] = "Gone\n";
					_errorCode[411] = "Length Required\n";
					_errorCode[412] = "Precondition Failed\n";
					_errorCode[413] = "Payload Too Large\n";
					_errorCode[414] = "URI Too Long\n";
					_errorCode[415] = "Unsupported Media Type\n";
					_errorCode[429] = "Too Many Requests\n";
				}
				virtual const char *what() const throw();
		}
};

#endif