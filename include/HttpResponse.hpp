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
		ServerBlock							*_server_block;
		LocationBlock						*_location_block;
		bool								_isResponseSent;
		std::string							_responseContent;
		std::time_t		 					_lastActionTimeStamp;
		std::map<std::string, std::string>	_mimeMap;
	
		void				_generateMimeMap(void);
		void 				_generateResponseContent(void);
		void 				_generateGETResponse(void);
		void 				_generatePOSTResponse(void);
		void 				_generateDELResponse(void);
		void				_generateErrorResponse(int errorCode, const char *errorMessage);
		void				_generateGenericErrorResponse(int errorCode, const char *errorMessage);
		void				_generateDirlistingResponse(std::string path);
		std::string			_displayTimeStamp(void);
		void				_fetchServerBlock(void);
		void				_fetchLocationBlock(void);
		void				_checkAllowedMethod(void);
		bool				_checkAcceptedFormat(std::string path);
		void				_fetchDirectoryRessource(std::string path);
		void				_fetchGETResource(void);
		void				_redirectOutput(void);
		bool 				_isPathWithinRoot(std::string path);
		// bool 			_isFileAboveThreshold(std::string &path);
		// void				_generateChunkedGETResponseContent(std::string path);
		void				_generateGETResponseContent(std::string path);
		std::string				_uploadFile(void);

	public:
		HttpResponse(Server &server, HttpRequest &request) : _server(server), _request(request),_isResponseSent(false), _lastActionTimeStamp(std::time(0))  {_generateMimeMap();_server_block = 0;_location_block = 0;};
		HttpResponse(HttpResponse const & rhs) : _server(rhs._server), _request(rhs._request) {if (this != &rhs) *this = rhs;};
		HttpResponse &operator=(HttpResponse const & rhs);
		~HttpResponse(void) {};
		
		bool getResponseStatus(void) const {return _isResponseSent;};
		void setResponseStatustoTrue(void);
		std::time_t getLastActionTimeStamp(void) const {return _lastActionTimeStamp;};
		std::string getResponseContent(void);
		HttpRequest & getRequest() const {return _request;};
		ServerBlock & getServerBlock() const {return *_server_block;};
		LocationBlock & getLocationBlock() const {return *_location_block;};

		class ServerError : public std::exception {
			private:
				int							_errorCode;
				std::map<int, const char*>	_errorMap;
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
				virtual ~ServerError() throw() {}; 
		};

		class ClientError : public std::exception {
			private:
				int							_errorCode;
				std::map<int, const char*>	_errorMap;
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
				virtual ~ClientError() throw() {};
		};
};

#endif
