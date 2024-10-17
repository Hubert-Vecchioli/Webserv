/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:00 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/17 18:14:12 by jblaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include "webserv.hpp"

class Server;
class ServerBlock;
class HttpRequest;
class LocationBlock;

class HttpResponse
{
	private:
		Server								&_server;
		HttpRequest							&_request;
		ServerBlock							&_server_block;
		LocationBlock						&_location_block;
		bool								_isResponseSent;
		std::string							_responseContent;
		std::time_t		 					_lastActionTimeStamp;
		std::map<std::string, std::string>	_mimeMap;
	
	private:
		void				_generateMimeMap(void);
		void 				_generateResponseContent(void);
		void 				_generateGETResponse(void);
		void 				_generatePOSTResponse(void);
		void 				_generateDELResponse(void);
		void				_generateErrorResponse(int errorCode, char *errorMessage);
		void				_generateGenericErrorResponse(int errorCode, char *errorMessage);
		void				_generateDirlistingResponse(void);
		std::stringstream	_displayTimeStamp(void);
		void				_fetchServerBlock(void);
		void				_fetchLocationBlock(void);
		void				_checkAllowedMethod(void);
		bool				_checkAcceptedFormat(std::string path);
		int					_fetchDirectoryRessource(std::string path);
		void				_fetchGETResource(void);

	public:
		HttpResponse(Server &server, HttpRequest &request) : _server(server), _request(request) {_generateMimeMap();};
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
					_errorMap[500] = "Internal Server Error";
					_errorMap[501] = "Not Implemented";
					_errorMap[502] = "Bad Gateway";
					_errorMap[503] = "Service Unavailable";
					_errorMap[504] = "Gateway Timeout";
					_errorMap[505] = "HTTP Version Not Supported";
				}
				int	getErrorCode() {return _errorCode;};
				virtual const char *what() const throw();
		};

		class ClientError : public std::exception {
			private:
				int							_errorCode;
				std::map<int, std::string>	_errorMap;
			public:
				ClientError(int code): _errorCode(code) {
					_errorMap[400] = "Bad Request";
					_errorMap[401] = "Unauthorized";
					_errorMap[403] = "Forbidden";
					_errorMap[404] = "Not Found";
					_errorMap[405] = "Method Not Allowed";
					_errorMap[406] = "Not Acceptable";
					_errorMap[407] = "Proxy Authentication Required";
					_errorMap[408] = "Request Timeout";
					_errorMap[409] = "Conflict";
					_errorMap[410] = "Gone";
					_errorMap[411] = "Length Required";
					_errorMap[412] = "Precondition Failed";
					_errorMap[413] = "Payload Too Large";
					_errorMap[414] = "URI Too Long";
					_errorMap[415] = "Unsupported Media Type";
					_errorMap[429] = "Too Many Requests";
				}
				int	getErrorCode() {return _errorCode;};
				virtual const char *what() const throw();
		};
};

#endif