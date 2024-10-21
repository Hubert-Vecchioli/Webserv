#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include "webserv.hpp"

class HttpResponse;

class CgiHandler
{
	public:
		CgiHandler(response.getRequest.getContentLength);
		CgiHandler(CgiHandler const & rhs);
		CgiHandler &operator=(CgiHandler const & rhs);
		~CgiHandler(void);
	
	private:
		std::map<std::string, std::string> _env;
							//"AUTH_TYPE" | "CONTENT_LENGTH" |
                        	//"CONTENT_TYPE" | "GATEWAY_INTERFACE" |
                        	//"PATH_INFO" | "PATH_TRANSLATED" |
                       	    //"QUERY_STRING" | "REMOTE_ADDR" |
                       	    //"REMOTE_HOST" | "REMOTE_IDENT" |
                       	    //"REMOTE_USER" | "REQUEST_METHOD" |
                       	    //"SCRIPT_NAME" | "SERVER_NAME" |
                       	    //"SERVER_PORT" | "SERVER_PROTOCOL" |
                       	    //"SERVER_SOFTWARE"

};

#endif //CGIHANDLER_HPP
